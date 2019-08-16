#ifndef EDITWORKHOURS_H
#define EDITWORKHOURS_H

#include <QDialog>
#include <QTime>

namespace Ui {
class EditWorkHours;
}

class EditWorkHours : public QDialog
{
    Q_OBJECT

public:

    bool cancelled = true;

    std::pair<QTime, QTime> result;

    explicit EditWorkHours(const QTime& _start, const QTime& _finish, const QString& _title = QString(), QWidget *parent = nullptr);
    ~EditWorkHours();

private slots:
    void on_pushButton_2_clicked();

    void on_timeEdit_userTimeChanged(const QTime &time);

    void on_timeEdit_2_userTimeChanged(const QTime &time);

private:
    Ui::EditWorkHours *ui;
};

#endif // EDITWORKHOURS_H
