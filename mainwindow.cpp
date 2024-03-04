#include "mainwindow.h"

#include <QObject>
#include <QtMath>

#include <QDebug>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)

  , precision{12}

  , num_first{0.0}
  , num_second{0.0}

  , isDarkTheme{false}
  , isTrigChecked{false}
  , isAdvancedChecked{false}
  , isRad{false}
  , isMathOperation{false}
  , isEvalPressed{false}

      , lastPressedArthOps{nullptr}

  , appHeight{491}
  , appWidth{303}
  , appWidthOneEnabled{372}
  , appWidthAllEnabled{441}
{
  ui->setupUi(this);
  ui->alg->hide();
  ui->trigonometry->hide();

  setColor();

  this->setFixedSize(appWidth, appHeight);

  this->setWindowTitle("Калькулятор");
  this->setStyleSheet("QLineEdit { padding-right: 12px; padding-left: 12px ; border: 0px ; background: " + displayBackColor + " ; }"
                      "QFrame { border: 0px; }"
                      "QFrame#display { background-color: "
                      + displayBackColor
                      + "; }"
                        "QFrame#keyboard { background-color: "
                      + keyboardBackColor
                      + "; }"
                        "QPushButton { border-radius: 30px; }"
                        "QPushButton:pressed { border-radius: 18px; }");
  // Раскрашиваю кнопки, отобр. арифм операции ---
  arthOpsStyle = "QPushButton { background-color: " + arthOpsBackColor + "; color: " + arthOpsTextColor +
  "; }"
                 "QPushButton:hover { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor + " ; }";

  pressedArthOpsStyle = "QPushButton { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor +
                        "; }"
                 "QPushButton:hover { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor + " ; border: none }"
      ;


  for (i = 0; i < ui->mainOperations->layout()->count() - 1; ++i) {
    it = qobject_cast<QPushButton *>(ui->mainOperations->layout()->itemAt(i)->widget());
    connect(it, &QPushButton::clicked, this, &MainWindow::mathOperations);
    it->setStyleSheet(arthOpsStyle);
  }
  for (i = 0; i < ui->controls->count(); ++i) {
      it = qobject_cast<QPushButton *>(ui->controls->itemAt(i)->widget());
      connect(it, &QPushButton::clicked, this, &MainWindow::operations);
      it->setStyleSheet(arthOpsStyle);
  }
  ui->pow->setStyleSheet(arthOpsStyle);
  // Кнопка = (eval)
  ui->eval->setStyleSheet(
      "QPushButton { background-color: " + evalButtonBackColor + "; color:  "  + evalButtonTextColor + "; }"
      "QPushButton:hover { background-color: "
      + hoveredEvalBackColor + "; color: " + evalButtonTextColor + "}");
  connect(ui->eval, &QPushButton::clicked, this, &MainWindow::mathOperations);
  // Возведение в степень
  connect(ui->pow, &QPushButton::clicked, this, &MainWindow::mathOperations);
  // Раскрашиваю кнопки, отобр цифры и изв мат. постоянные
  numsStyle = "QPushButton { background-color: " + displayBackColor
              + " ; color: " + displayTextColor
              + " ; }"
                "QPushButton:hover { background-color: "
              + onDisplayBackColor + " ; }";
  for (i = 0; i < ui->nums->count(); ++i) {
    it = qobject_cast<QPushButton *>(ui->nums->itemAt(i)->widget());
    connect(it, &QPushButton::clicked, this, &MainWindow::digitsNumbers);
    it->setStyleSheet(numsStyle);
  }
  ui->pi->setStyleSheet(numsStyle);
  connect(ui->pi, &QPushButton::clicked, this, &MainWindow::digitsNumbers);
  ui->e->setStyleSheet(numsStyle);
  connect(ui->e, &QPushButton::clicked, this, &MainWindow::digitsNumbers);
  // Алгебра и тригонометрия
  advancedMathStyle = "QPushButton { background-color: rgb(199, 236, 203); color: rgb(1, 33, "
                      "13); }"
                      "QPushButton:hover { background-color: rgb(171, 207, 176); }";
  for (i = 0; i < ui->trigonometry->layout()->count() - 1; ++i) {
      it = qobject_cast<QPushButton *>(ui->trigonometry->layout()->itemAt(i)->widget());
      it->setStyleSheet(advancedMathStyle);
  }
  for (i = 1; i < ui->alg->layout()->count() - 1; ++i) {
      it = qobject_cast<QPushButton *>(ui->alg->layout()->itemAt(i)->widget());
      it->setStyleSheet(advancedMathStyle);
  }

  connect(ui->log, &QPushButton::clicked, this, &MainWindow::mathFunctions);
  connect(ui->fact, &QPushButton::clicked, this, &MainWindow::mathFunctions);
  connect(ui->sqrt, &QPushButton::clicked, this, &MainWindow::mathFunctions);

  ui->secondaryText->setText("");

  ui->indicator->setText("ГРАД");

  // ---
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setColor()
{
    if (isDarkTheme) {
        // Темная тема
        displayBackColor = "rgb(61, 57, 47)";
        displayTextColor = "rgb(233,226,212)";
        keyboardBackColor = "rgb(22 , 19 , 11 )";
        controlsBackColor = "rgb(205, 232, 224)";
        controlsTextColor = "rgb(6, 32, 27)";
    } else {
        // Светлая тема
        displayBackColor = "rgb(225, 217, 204)";
        onDisplayBackColor = "rgb(195, 185, 162)";
        displayTextColor = "rgb(30, 27, 19)";
        keyboardBackColor = "rgb(255, 248, 240)";
        controlsBackColor = "rgb(177, 204, 196)";
        controlsTextColor = "rgb(51, 75, 70)";

        evalButtonTextColor = "rgb(113, 92, 13)";
        evalButtonBackColor = "rgb(253, 225, 134)";
        hoveredEvalBackColor = "rgb(224, 197, 109)";

        arthOpsBackColor = "rgb(255, 217, 226)";
        arthOpsTextColor = "rgb(49, 19, 28)";
        pressedArthOpsBackColor = "rgb(150, 103, 117)";
        pressedArthOpsTextColor = "rgb(224, 197, 109)";
        hoveredArthOpsTextColor = "rgb(245, 249, 237)";
    }
}

void MainWindow::on_showAgvancedAction_toggled(bool arg1)
{
    if (arg1) {
        isAdvancedChecked = true;
        ui->alg->show();
        if (isTrigChecked) {
            this->setFixedSize(appWidthAllEnabled, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    } else {
        isAdvancedChecked = false;
        ui->alg->hide();
        if (!isTrigChecked) {
            this->setFixedSize(appWidth, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    }
}

void MainWindow::on_showTrigAction_toggled(bool arg1)
{
    if (arg1) {
        isTrigChecked = true;
        ui->trigonometry->show();
        if (isAdvancedChecked) {
            this->setFixedSize(appWidthAllEnabled, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    } else {
        isTrigChecked = false;
        ui->trigonometry->hide();
        if (!isAdvancedChecked) {
            this->setFixedSize(appWidth, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    }
}


void MainWindow::digitsNumbers()
{
  // all_numbers :: введенные числа
  // new_label :: строка, которая отображается на основном дисплее
  QPushButton *button = (QPushButton *)sender();
  double all_numbers;
  QString new_label = ui->primaryText->text();
  if (button == ui->del) {
    if (isMathOperation) {
      ui->del->setText("AC");
      if (lastPressedArthOps) {
        lastPressedArthOps->setStyleSheet(arthOpsStyle);
        lastPressedArthOps = nullptr;
      }
      isMathOperation = false;
    } else {
      isEvalPressed = false;

      ui->dot->setEnabled(true);
      new_label = "0";
    }
    ui->secondaryText->setText("");
  } else {
    if (button->text() == ".") {
      new_label = (ui->primaryText->text().contains(".") ? ui->primaryText->text() : ui->primaryText->text() + button->text());
      ui->dot->setEnabled(false);
    } else {
      new_label = (ui->primaryText->text() == "0" ? button->text() : ui->primaryText->text() + button->text());
    }
  }
  ui->primaryText->setText(new_label);
}

void MainWindow::evaluateResult() {

  if (mathOperationSign == "+") {
    mathOperationRes = QString::number(num_first + num_second, 'g', precision);
  } else if (mathOperationSign == "-") {
    mathOperationRes = QString::number(num_first - num_second, 'g', precision);
  } else if (mathOperationSign == "×") {
    mathOperationRes = QString::number(num_first * num_second, 'g', precision);
  } else if (mathOperationSign == "/") {
    if (num_second) {
      mathOperationRes = QString::number(num_first / num_second, 'g', precision);
    } else {
      mathOperationRes = "NaN";
    }
  } else if (mathOperationSign == "^") {
    mathOperationRes = QString::number(qPow(num_first, num_second), 'g', precision);
  }
  ui->primaryText->setText(mathOperationRes);
  ui->secondaryText->setText(QString::number(num_first) + mathOperationSign + QString::number(num_second, 'g', precision) + " =");

}

void MainWindow::mathOperations()
{
  QPushButton *button = (QPushButton *)sender();

  // ---
  // Если у нас выбрана хотябы одна из операций, то мы при нажатии на eval ее выполняем, иначе не делаем ничего.
  // При повторном нажатии на знак операции он должен вести себя как "="
  // При повторном нажатии на знак "=" в левым операндом должен стать результат операции.
  // Мат Операция должна заканчиваться только в случае если либо нажата "DEL", либо введено обычное число.
  if (isMathOperation) {
    if (isEvalPressed) {

      if (button == ui->eval) {
        num_first = ui->primaryText->text().toDouble();
        evaluateResult();

      } else if (button == ui->plus || button == ui->minus || button == ui->mult || button == ui->div || button == ui->pow) {
        isEvalPressed = false;
        num_first = mathOperationRes.toDouble();

        mathOperationSign = button->text();

        ui->primaryText->setText("0");
        ui->secondaryText->setText(QString::number(num_first, 'g', precision) + " " + mathOperationSign + " ");

      }

    } else {
      if (button == ui->plus || button == ui->minus || button == ui->mult || button == ui->div || button == ui->pow) {
        num_second = ui->primaryText->text().toDouble();
        mathOperationSign = button->text();

        evaluateResult();

        isMathOperation = true;
        ui->del->setText("C");
        button->setStyleSheet(pressedArthOpsStyle);
        lastPressedArthOps = button;

      } else if (button == ui->eval) {
        num_second = ui->primaryText->text().toDouble();
        evaluateResult();

        isEvalPressed = true;
      }
    }
  } else {

    if (ui->primaryText->text() == "π") {
      num_first = M_PI;
    } else if (ui->primaryText->text() == "e") {
      num_first = M_E;
    } else {
      num_first = ui->primaryText->text().toDouble();
    }

    if (button == ui->plus || button == ui->minus || button == ui->mult || button == ui->div || button == ui->pow) {
      // Прописать логику для повторного нажатия на ту же операцию

      mathOperationSign = button->text();

      ui->primaryText->setText("0");
      ui->secondaryText->setText(QString::number(num_first, 'g', precision) + " " + mathOperationSign + " ");

      isMathOperation = true;

    } else if (button == ui->eval) { // Оставляет тоже число на дисплее.
      ui->primaryText->setText(QString::number(num_first, 'g', precision));
    }

  }
}

int MainWindow::fact(int n) {
  if ((n==0)||(n==1))
    return 1;
  else
    return n*fact(n-1);
}

void MainWindow::mathFunctions() {
  QPushButton *button = (QPushButton *)sender();
  if (button == ui->sqrt) {
    mathOperationRes = QString::number(qSqrt(ui->primaryText->text().toDouble()), 'g', precision);
    ui->secondaryText->setText("sqrt(" + ui->primaryText->text() + ") =");
  } else if (button == ui->fact) {
    mathOperationRes = QString::number(fact(ui->primaryText->text().toInt()));
    ui->secondaryText->setText(ui->primaryText->text() + "! =");
  } else if (button == ui->log) {
    num_first = ui->primaryText->text().toDouble();
    if (num_first > 0) {
      mathOperationRes = QString::number(qLn(num_first));
    } else {
      mathOperationRes = "NaN";
    }
    ui->secondaryText->setText("ln("+ ui->primaryText->text() + ") =");
  }
  ui->primaryText->setText(mathOperationRes);
}

void MainWindow::operations()
{
  QPushButton *button = (QPushButton *)sender();
  double all_numbers;
  if(button == ui->changeSign) {
    all_numbers = (ui->primaryText->text().toDouble());
    all_numbers = all_numbers * -1;
    QString new_label = QString::number(all_numbers, 'g', 12);
    ui->primaryText->setText(new_label);
  } else if(button == ui->percent) {
    all_numbers = (ui->primaryText->text().toDouble());
    all_numbers = all_numbers * 0.01;
    QString new_label = QString::number(all_numbers, 'g', 12);
    ui->primaryText->setText(new_label);
  } else if (button == ui->toRad) {
    if (!isRad) {
      isRad = true;
      ui->toRad->setText("ГРАД");
      ui->indicator->setText("РАД");

    } else {
      isRad = false;
      ui->toRad->setText("РАД");
      ui->indicator->setText("ГРАД");
    }
  }
}

double MainWindow::degToRad() {
  num_first = ui->primaryText->text().toDouble();
  return (isRad ? num_first : qDegreesToRadians(num_first));
}

void MainWindow::on_cos_clicked()
{
  ui->cos->setChecked(true);
    // Вычислите косинус в градусах
    double labelNumber = qCos(degToRad());
    QString new_label = QString::number(labelNumber, 'g', precision);
    ui->primaryText->setText(new_label); // Вывод результата косинуса
    ui->cos->setChecked(false);
    // Сброс значения num_first после вычисления косинуса
    num_first = 0;
}

void MainWindow::on_sin_clicked()
{
  ui->sin->setChecked(true);
  // Вычислите синус в градусах
  double labelNumber = qSin(degToRad());
  QString new_label = QString::number(labelNumber, 'g', precision);
  ui->primaryText->setText(new_label); // Вывод результата синуса
  ui->sin->setChecked(false);

  // Сброс значения num_first после вычисления синуса
  num_first = 0;
}


void MainWindow::on_tg_clicked()
{
  ui->tg->setChecked(true);
  // Вычислите тангенс в градусах
  double labelNumber = qTan(degToRad());
  QString new_label = QString::number(labelNumber, 'g', precision);
  ui->primaryText->setText(new_label); // Вывод результата тангенса
  ui->tg->setChecked(false);

  // Сброс значения num_first после вычисления тангенса
  num_first = 0;
}


void MainWindow::on_ctg_clicked()
{
  // Вычислите котангенс в градусах
  double labelNumber = 1.0 / qTan(degToRad());
  QString new_label = QString::number(labelNumber, 'g', precision);
  ui->primaryText->setText(new_label); // Вывод результата котангенса
  ui->ctg->setChecked(false);

  // Сброс значения num_first после вычисления котангенса
  num_first = 0;
}

// void MainWindow::on_del_clicked() {

// }
