#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include "storagesettingdialog.h" // 记得引入弹窗的头文件
#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //这里调用了save，然后把ui和这里的代码联系上了
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::handleSaveAction);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::handleClearAction);

    // 假设你在 UI 设计里创建的动作名字叫 actionSettings
    // 这一行非常关键，否则你点了菜单栏也不会有反应
    connect(ui->actionStoragePathSetting, &QAction::triggered, this, &MainWindow::openSettingsDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::handleSaveAction() {

    QString basePath = getStorageBasePath();

    ensureStorageDirectoryExists(basePath);

    QString defaultFullPath = basePath + "/editerCalendar_recordings/" + generateFileName();
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", defaultFullPath, "文本文件 (*.txt);;所有文件 (*.*)");

    if (fileName.isEmpty()) {
        return false;
    }

    // 调用通用的保存函数
    return saveFile(fileName);
}

// 新增：专门处理自动保存逻辑
bool MainWindow::autoSaveAction() {
    // 1. 获取用户路径
    QString basePath = getStorageBasePath();

    // 2. 确保文件夹存在
    ensureStorageDirectoryExists(basePath);

    // 3. 自动生成路径
    QString targetPath = basePath + "/editerCalendar_recordings/" + generateFileName();

    // 4. 执行写入
    return saveFile(targetPath);
}


void MainWindow::handleClearAction(){
    //确认的提示
    // int ret = QMessageBox::warning(this, "警告", "确定要清空文本吗？未保存的内容将丢失。", QMessageBox::Yes | QMessageBox::No);

    //改为调用方法确认是否要清除
    if (!maybeSave()) {
        return;
    }

    //清楚的代码
    ui->textEdit->clear();

    // 清空后，别忘了标记为“未修改”，否则下一次关闭时还会弹窗
    ui->textEdit->document()->setModified(false);

}


bool MainWindow::maybeSave() {
    if (!ui->textEdit->document()->isModified()) {
        return true;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "提示",
                               "当前内容尚未保存，是否保存？",
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    // 修复点：明确处理每一种情况
    if (ret == QMessageBox::Save) {
        return autoSaveAction();
    } else if (ret == QMessageBox::Cancel) {
        // 关键修复：点击取消，必须返回 false，终止后续的操作（如清空或关闭）
        return false;
    }

    // 如果是 Discard（放弃保存），自然会执行到这里，返回 true，表示允许执行清空操作
    return true;
}



bool MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开并写入该文件");
        return false; // 保存失败
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();


    // 关键：告诉Qt“现在文件已经保存好了，不是被修改过的状态”
    // 这样下次就不会再弹窗烦你了！
    ui->textEdit->document()->setModified(false);

    ui->statusbar->showMessage("文件保存成功！", 3000);
    return true; // 保存成功
}

//储存用户设置的保存路径
void MainWindow::openSettingsDialog() {
    StorageSettingDialog dlg(this); // 1. 创建弹窗实例

    if (dlg.exec() == QDialog::Accepted) { // 2. 如果用户在弹窗里点了“确定”

        // 3. 【获取】从弹窗的出口函数拿到了用户选好的新路径
        QString newPath = dlg.getSelectedPath();

        // 4. 【保存】统一用 "EditerCalender" 这个小本本永久写入硬盘
        QSettings settings("MyCompany", "EditerCalender");
        settings.setValue("storagePath", newPath);

        ui->statusbar->showMessage("存储路径已更新", 2000);
    }
}


//自动给一个名字
QString MainWindow::generateFileName() {
    // 格式：2026-06-07_18-30-05.txt
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    return timeStr + ".txt";
}



void MainWindow::ensureStorageDirectoryExists(const QString &basePath) {
    QString fullPath = basePath + "/editerCalendar_recordings";
    QDir dir;
    if (!dir.exists(fullPath)) {
        dir.mkpath(fullPath); // 如果文件夹不存在，自动创建（支持多级目录）
    }
}


// 获取用户的保存路径
QString MainWindow::getStorageBasePath() const {
    QSettings settings("MyCompany", "EditerCalender");
    return settings.value("storagePath", QCoreApplication::applicationDirPath()).toString();
}

