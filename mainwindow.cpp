#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editworkhours.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QStandardPaths>
#include <QDesktopWidget>

#include <fstream>
#include <memory>
#include <array>

#include "xlsxdocument.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_import_2_triggered()
{
    if(ui->maxi->isChecked())
        this->showMaximized();

    //prepare directory for storage
    try{ fs::remove(fs::path{__mTemporaryDirectory}); }catch(...) {}
    fs::create_directory(fs::path{__mTemporaryDirectory});

    //index for temporary files
    uint8_t indexer = 0;

    //iterate over all files in /var/logs
    for(const auto& var : fs::directory_iterator(__mPathToLogsDirectory))
    {
        //if file name have 'auth.lo' in name continue
        if(var.path().string().find(__mAuthRegex) != std::string::npos)
        {
            //prepare new name
            std::string targetName{ "/auth"+std::to_string(indexer)+".log" };

            //increment indexer (very important comment)
            indexer++;

            //if compressed, add appropiate sufix
            if(var.path().string().find("gz") != std::string::npos)
                targetName += ".gz";

            //copy selected file
            fs::copy(var.path(), __mTemporaryDirectory+targetName, fs::copy_options::overwrite_existing);

            //if previously added sufix exist, decompress
            if(targetName.find("gz") != std::string::npos)
                std::system((__mUnzipCommand+__mTemporaryDirectory+targetName).c_str());
        }        
    }

    //get device name
    std::string deviceName;
    std::system("uname -n >> name");
    std::getline(std::ifstream("name"), deviceName);

    try{ fs::remove(fs::path{"./finalForm"}); }catch(...) {}

    //transfer everythink to one file
    for(uint8_t i = 0; i < indexer; i++)
    {
        std::string tmp("cat " + __mTemporaryDirectory + "/auth"+std::to_string(i)+".log | cut -d '"+
                        deviceName[0]+R"(' -f 1 | uniq | sed -e 's/  / 0/g' | tr '\n' ' ' | tr -d '\0' | sed -e 's/  /\n/g' | uniq >> ./finalForm)");
//        cout << tmp.c_str();
        std::system(tmp.c_str());
    }

    //set tokenizer and prepare read
    const boost::char_separator<char> sep{" :\n\r"};
    std::ifstream logs("./finalForm");

    //17 is amount of bytes per one line
    const size_t bufferSize = 16;

    //create buffer
    char * buffer = new char[bufferSize];

    //prepare space for storing
    std::array<std::string, 5> single_row;

    ulli ite = 0;

    //read line by line
    while(logs)
    {
        logs.read(buffer, bufferSize);
        ite++;
        //tokenize collected line
        std::string temporary{buffer, bufferSize};
        tokenizer tokens{temporary, sep};
        size_t i= 0;
        for(const auto& tok : tokens)
        {
            single_row[i].assign(tok.data());
            i++;
            if(i == 5) break;
        }        
        //process acquired data
        flag proccessable = true;
        for(const auto& var : single_row)
            if(var == "" || var == " ") proccessable = false;

        if(!proccessable) continue;

        process(QDate(QDate::currentDate().year(), __mMonths.at(single_row[0]), std::stoi(single_row[1])),
                QTime(std::stoi(single_row[2]), std::stoi(single_row[3]), std::stoi(single_row[4])));
    }

    //clear memory, no more need to keep it
    delete[] buffer;
    buffer = nullptr;

    //prepare GUI
    ui->records->setColumnCount(5);
    ui->records->setHorizontalHeaderLabels({ "Day", "Started", "Finished", "Hours", "Overhours [ 8h ]" });

    //prepare seconds counter
    total_time_in_seconds = 0;

    //iterate over all days and display it
    for(const auto& var : __mRecors)
    {
        const int rows = ui->records->rowCount();
        ui->records->insertRow(rows);
        auto it = __mBackup.insert(std::pair<int, QStringList>{rows, QStringList{
                QString{var.first.toString("MM-dd")},
                QString{var.second.first.toString("hh:mm:ss")},
                QString{var.second.second.toString("hh:mm:ss")},
                substractTime(var.second.first, var.second.second),
                calculateOverhours(var.second.first, var.second.second)
                            }});

        ui->records->setItem(rows, 0, new QTableWidgetItem{it.first->second[0]});
        ui->records->setItem(rows, 1, new QTableWidgetItem{it.first->second[1]});
        ui->records->setItem(rows, 2, new QTableWidgetItem{it.first->second[2]});
        ui->records->setItem(rows, 3, new QTableWidgetItem{it.first->second[3]});
        ui->records->setItem(rows, 4, new QTableWidgetItem{it.first->second[4]});
    }
}

void MainWindow::on_export_2_triggered()
{
    //I don't know is it possible to make more useless functionality
    if(ui->maxi->isChecked())
        this->showMaximized();

    //Create document in home location
    QXlsx::Document doc(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+
                "/resume_"+QDate::currentDate().toString("yyyy_MM_dd")+".xlsx");

    //Create some ready to use formats
    QXlsx::Format header;
    header.setFontBold(true);
    header.setFontSize(13);
    header.setBorderStyle(QXlsx::Format::BorderStyle::BorderThick);
    header.setBorderIndex(2);
    header.setNumberFormat("@");
    header.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignHCenter);

    QXlsx::Format stnd;
    stnd.setFontSize(12);
    stnd.setBorderIndex(1);
    stnd.setTopBorderStyle(QXlsx::Format::BorderStyle::BorderNone);
    stnd.setBottomBorderStyle(QXlsx::Format::BorderStyle::BorderNone);
    stnd.setNumberFormat("@");
    stnd.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignHCenter);

    QXlsx::Format stnd_date{stnd};
    stnd_date.setNumberFormat("D.MM.YYYY");

    QXlsx::Format stnd_time{stnd};
    //stnd_time.setNumberFormat("@"); // <- This on is now commented due to negative time, but in near (lie) future it will be upgraded

    //Write down headers
    doc.write(1, 1, "Day", header);
    doc.write(1, 2, "Started", header);
    doc.write(1, 3, "Finished", header);
    doc.write(1, 4, "Hours", header);
    doc.write(1, 5, "Over Hours [ 8h ]", header);

    //Write all data
    int row = 2;
    for( const auto& var : __mBackup )
    {
        if(row + 1 == static_cast<int>(__mRecors.size()))
        {
            stnd.setBottomBorderStyle(stnd.leftBorderStyle());
            stnd_date.setBottomBorderColor(stnd_date.leftBorderStyle());
            stnd_time.setBottomBorderColor(stnd_time.leftBorderStyle());
        }

        doc.write(row, 1, var.second[0], stnd_date);
        doc.write(row, 2, var.second[1], stnd_time);
        doc.write(row, 3, var.second[2], stnd_time);
        doc.write(row, 4, substractTime(QTime::fromString(var.second[1], "hh:mm:ss"), QTime::fromString(var.second[2], "hh:mm:ss")), stnd_time);
        doc.write(row, 5, calculateOverhours(QTime::fromString(var.second[1], "hh:mm:ss"), QTime::fromString(var.second[2], "hh:mm:ss")), stnd_time);
        row++;
    }

    //Small summarry
    doc.mergeCells(QXlsx::CellRange(row, 1, row, 4), stnd);
    stnd.setNumberFormat("#");
    total_time_in_seconds = 0;
    for(size_t i = 0; i < __mBackup.size(); i++)
        total_time_in_seconds += resumeTime(QTime::fromString(__mBackup[i][1]), QTime::fromString(__mBackup[i][2]));
    doc.write(row, 5, (3600 * 8 * __mRecors.size()) - total_time_in_seconds, stnd);
    stnd.setNumberFormat("@");
    stnd.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignLeft);
    doc.write(row, 1, "Total over hours [s]: ", stnd);

    //Column resize
    for(int i = 1; i <= 5; i++)
        doc.setColumnWidth(i, 30);

    //Save all stuff above
    doc.save();

    //Amen.
    msgBox("Done. Saved in your home directory.");
}

void MainWindow::on_about_triggered()
{
    msgBox({"As a programmer i feel sick if stuff like this is not automated","\nCreated by: Krzysztof Mochoki", "", "I promise to add some settings in near future", "", "\nPS: In 2019 C++ still on top ;)"});
}

void MainWindow::msgBox(const QString &src) const noexcept
{
    QMessageBox msg(QMessageBox::Icon::NoIcon, "Info", src);
    msg.show();
    msg.topLevelWidget();
    msg.exec();
}

void MainWindow::msgBox(const QStringList &src) const noexcept
{
    QString finalMsg = "";
    for(const auto& var : src)
    {
        finalMsg += var;
        finalMsg += "\n";
    }
    msgBox(finalMsg);
}

void MainWindow::process(const QDate &date, const QTime &time) noexcept
{
    //find date
    auto result = __mRecors.find(date);

    //if found
    if(result != __mRecors.end())
    {
       minmax& actual = result->second;

       //comparing
       if(time < actual.first) actual.first = time;
       if(time > actual.second) actual.second = time;

       //saving result
       result->second = actual;
    }
    else __mRecors[date] = minmax{ time, time };

}

QString MainWindow::substractTime(const QTime &start, const QTime &stop) const noexcept
{
    int hours = stop.hour() - start.hour();
    int minutes = stop.minute() - start.minute();
    int seconds = stop.second() - start.second();
    if(hours >= 0 && minutes >= 0 && seconds >= 0) return QTime(hours, minutes, seconds).toString("hh:mm:ss");
    if(seconds < 0)
    {
        seconds += 60;
        minutes--;
        if(hours >=0 && minutes >= 0) return QTime(hours, minutes, seconds).toString("hh:mm:ss");
    }

    if(minutes < 0)
    {
        minutes += 60;
        hours--;
        if(hours >= 0) return QTime(hours, minutes, seconds).toString("hh:mm:ss");
    }

    if(hours < 0) return QString(QString::number(hours)+":"+QString::number(minutes)+":"+QString::number(seconds));
    else return QTime(hours, minutes, seconds).toString("hh:mm:ss");
}

QString MainWindow::calculateOverhours(const QTime &start, const QTime &stop, const QTime day_work_time) noexcept
{
    const ulli tmp = resumeTime(start, stop);

    if( tmp > static_cast<ulli>((day_work_time.hour() * 3600) + (day_work_time.minute() * 60) + day_work_time.second()))
        return substractTime(day_work_time, ulli_to_QDate(tmp));
    else
        return "-" + substractTime(ulli_to_QDate(tmp), day_work_time);
}

ulli MainWindow::resumeTime(const QTime &start, const QTime &stop) const
{
    int hours = stop.hour() - start.hour();
    int minutes = stop.minute() - start.minute();
    int seconds = stop.second() - start.second();
    if(seconds < 0)
    {
        seconds += 60;
        minutes--;
    }

    if(minutes < 0)
    {
        minutes += 60;
        hours--;
    }

    return static_cast<ulli>((hours * 3600) + (minutes * 60) + seconds);

}

QTime MainWindow::ulli_to_QDate(const ulli& src1) const
{
    int src = static_cast<int>(src1);
    int hours = src / 3600;
    src -= hours * 3600;
    int minutes = src / 60;
    src -= minutes * 60;
    return QTime{ hours, minutes, src };
}

void MainWindow::on_records_itemDoubleClicked(QTableWidgetItem *)
{
    const int row = ui->records->selectedItems().front()->row();
    if(row < 0) return;

    auto it = __mBackup.find(row);

    std::unique_ptr<EditWorkHours> tmp{ new EditWorkHours{
                    QTime::fromString(it->second[1], "hh:mm:ss"),
                    QTime::fromString(it->second[2], "hh:mm:ss"),
                    "Edit: " + it->second[0], this } };
    tmp->show();
    tmp->topLevelWidget();
    tmp->exec();

    if(tmp->cancelled) return;

    it->second = QStringList{ it->second[0], tmp->result.first.toString("hh:mm:ss"),
            tmp->result.second.toString("hh:mm:ss"), substractTime(tmp->result.first, tmp->result.second),
            calculateOverhours(tmp->result.first, tmp->result.second) };

    ui->records->setItem(row, 1, new QTableWidgetItem{it->second[1]});
    ui->records->setItem(row, 2, new QTableWidgetItem{it->second[2]});
    ui->records->setItem(row, 3, new QTableWidgetItem{it->second[3]});
    ui->records->setItem(row, 4, new QTableWidgetItem{it->second[4]});
}
