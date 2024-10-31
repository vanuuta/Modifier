#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qDebug() << "Главное окно инициализировано."; // Отладочный вывод
    ui->comboBox->addItems(QStringList() << "Перезаписать" << "Модифицировать");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString inputMask = ui->lineEdit->text();
    QString outputDir = ui->lineEdit_2->text();
    bool deleteAfterProcessing = ui->checkBox->isChecked();
    QString overwriteMode = ui->comboBox->currentText();
    int pollingInterval = ui->spinBox->value();
    QByteArray xorValue = QByteArray::fromHex(ui->lineEdit_3->text().toUtf8());
    QDir dir(outputDir);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Ошибка", "Указанный путь для сохранения не существует!");
        return;
    }

    fileModifier = new FileModifier(inputMask, deleteAfterProcessing, outputDir,
                                   overwriteMode, pollingInterval, xorValue);
    // QString inputMask = "*.txt"; // маска входных файлов
    // bool deleteAfterProcessing = false; // удалять или нет
    // QString outputDir = "D:/qt/FileModifier/res"; // путь для сохранения файлов
    // QString overwriteMode = "Перезаписать"; // перезапись или модификация
    // int pollingInterval = 1; // периодичность опроса в секундах
    // //QByteArray xorValue = QByteArray::fromHex("0xAA"); // 8-байтное значение

    // FileModifier FileModifier(inputMask, deleteAfterProcessing, outputDir,
    //                           overwriteMode, pollingInterval, key);
    //
    QMessageBox::information(this, "Запуск", "Обработка файлов началась!");
}

