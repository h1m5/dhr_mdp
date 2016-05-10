#ifndef NEWCITY_H
#define NEWCITY_H

#include <QDialog>

namespace Ui {
    class NewCity;
}

class NewCity : public QDialog
{
    Q_OBJECT

public:
    explicit NewCity(QWidget *parent = 0);
    ~NewCity();

    QString cityName() const;

signals:

private slots:
    void on_okButton_clicked();

private:
    Ui::NewCity *ui;
    QString _cityName;
};

#endif // NEWCITY_H
