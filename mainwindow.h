#pragma once

#include <QMainWindow>
#include <map>
#include <boost/tokenizer.hpp>
#include <experimental/filesystem>
#include <QDate>
#include <QTime>
#include <QDebug>

#include <QTableWidgetItem>

#define cout qDebug()

namespace fs = std::experimental::filesystem;

using tokenizer = boost::tokenizer<boost::char_separator<char> >;
using minmax = std::pair<QTime, QTime>;
using flag = bool;

using ulli = unsigned long long int;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    const std::string __mPathToLogsDirectory{"/var/log"};
    const std::string __mAuthRegex{"auth.lo"};
    const std::string __mUnzipCommand{"gzip -d "};
    const std::string __mTemporaryDirectory{"/tmp/auth_logs_process"};
    const std::map<std::string, int> __mMonths{ {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},
                                                    {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8},
                                                    {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
                                                  };

    std::map<QDate, minmax> __mRecors;
    std::map<int, QStringList> __mBackup;

    ulli total_time_in_seconds = 0;
    flag first_cycle;

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_import_2_triggered();

    void on_export_2_triggered();

    void on_about_triggered();

    void on_records_itemDoubleClicked(QTableWidgetItem *);

private:

    Ui::MainWindow *ui;

    void msgBox(const QString& src) const noexcept;

    void msgBox(const QStringList& src) const noexcept;

    void process(const QDate& date, const QTime& time) noexcept;

    QString substractTime(const QTime& start, const QTime& stop) const noexcept;

    QString calculateOverhours(const QTime& start, const QTime& stop, const QTime day_work_time = QTime( 8, 0, 0)) noexcept;

    ulli resumeTime(const QTime &start, const QTime &stop) const;
    QTime ulli_to_QDate(const ulli& src) const;
};
