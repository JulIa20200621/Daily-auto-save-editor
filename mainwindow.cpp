#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //这里调用了save，然后把ui和这里的代码联系上了
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::handleSaveAction);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::handleClearAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::handleSaveAction(){
    //获取保存文件的位置
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "文本文件 (*.txt);;所有文件 (*.*)");

    //如果取消了，直接返回empty
    if (fileName.isEmpty()) {
        return;
    }

    //创建文件
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 如果打开失败，弹窗警告并结束
        QMessageBox::warning(this, "错误", "无法打开并写入该文件");
        return;
    }

    //写入ui里面的文字
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();


    //保存并提示
    file.close(); // 显式关闭文件是一个好习惯
    ui->statusbar->showMessage("文件保存成功！", 3000); // 在状态栏显示 3 秒提示

    //保存进列表
    // m_savedFiles.prepend(fileName);

    //重复保存去掉重复路径
    // m_savedFiles.removeDuplicates();

    //更新显示列表
    // updateSavedFilesLayout();

}



void MainWindow::handleClearAction(){
    //确认的提示
    int ret = QMessageBox::warning(this, "警告", "确定要清空文本吗？未保存的内容将丢失。", QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        ui->textEdit->clear(); // 这就是那个“魔法一行代码”
    }

}

