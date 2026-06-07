#ifndef STORAGESETTINGDIALOG_H
#define STORAGESETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class StorageSettingDialog;
}

class StorageSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StorageSettingDialog(QWidget *parent = nullptr);
    ~StorageSettingDialog();

private:
    Ui::StorageSettingDialog *ui;
};

#endif // STORAGESETTINGDIALOG_H
