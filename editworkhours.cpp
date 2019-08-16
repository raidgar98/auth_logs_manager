#include "editworkhours.h"
#include "ui_editworkhours.h"

EditWorkHours::EditWorkHours(const QTime& _start, const QTime& _finish, const QString& _title, QWidget *parent) :
    result{std::make_pair(_start, _finish)},
    QDialog(parent),
    ui(new Ui::EditWorkHours)
{
    ui->setupUi(this);
    setWindowTitle( _title == QString() ? "Change work hours" : _title );
    ui->timeEdit->setTime(result.first);
    ui->timeEdit_2->setTime(result.second);
}

EditWorkHours::~EditWorkHours()
{
    delete ui;
}

void EditWorkHours::on_pushButton_2_clicked()
{
    cancelled = false;
    this->close();
}

void EditWorkHours::on_timeEdit_userTimeChanged(const QTime &time)
{
    result.first = time;
}

void EditWorkHours::on_timeEdit_2_userTimeChanged(const QTime &time)
{
    result.second = time;
}
