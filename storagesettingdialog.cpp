#include "storagesettingdialog.h"
#include "ui_storagesettingdialog.h"
#include <QFileDialog>
#include <QSettings>

StorageSettingDialog::StorageSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StorageSettingDialog)
{
    ui->setupUi(this);

    // 1. 读取旧路径并显示（如果已经设置过）
    QSettings settings("MyCompany", "EditerCalender");
    QString path = settings.value("storagePath", "").toString();
    ui->pathLineEdit->setText(path);

    // 2. 绑定“浏览”按钮点击事件
    connect(ui->browseButton, &QPushButton::clicked, this, [=](){
        QString dir = QFileDialog::getExistingDirectory(this, "选择文件夹", ui->pathLineEdit->text());
        if (!dir.isEmpty()) {
            ui->pathLineEdit->setText(dir);
        }
    });

}


StorageSettingDialog::~StorageSettingDialog()
{
    delete ui;
}

QString StorageSettingDialog::getSelectedPath() const {
    return ui->pathLineEdit->text(); // 直接把界面输入框里的文本返回出去
}

