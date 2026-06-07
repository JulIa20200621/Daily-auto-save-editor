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
    QSettings settings("MyCompany", "EditerCalender"); // 建议用你的项目名
    QString path = settings.value("storagePath", "").toString();
    ui->pathLineEdit->setText(path);

    // 2. 绑定“浏览”按钮点击事件
    connect(ui->browseButton, &QPushButton::clicked, this, [=](){
        QString dir = QFileDialog::getExistingDirectory(this, "选择文件夹", ui->pathLineEdit->text());
        if (!dir.isEmpty()) {
            ui->pathLineEdit->setText(dir);
        }
    });

    // 3. 绑定“OK”按钮点击后的保存逻辑
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=](){
        QSettings settings("MyCompany", "EditerCalender");
        settings.setValue("storagePath", ui->pathLineEdit->text());
    });
}

StorageSettingDialog::~StorageSettingDialog()
{
    delete ui;
}

