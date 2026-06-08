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
    // 新增：让主窗口调用的公开接口，用来返回用户选好的路径
    QString getSelectedPath() const;

private:
    Ui::StorageSettingDialog *ui;

};

#endif // STORAGESETTINGDIALOG_H
