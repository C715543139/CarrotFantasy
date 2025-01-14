#include "SelectWidget.h"
#include "ui_SelectWidget.h"

SelectWidget::SelectWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SelectWidget) {
    ui->setupUi(this);

    // 选关背景
    ui->background->setStyleSheet("background-image: url(:/res/SelectScene/selectBg.png);");

    // 按钮背景
    setBtnBackground(ui->leftBtn, ":/res/SelectScene/leftBtn.png", ":/res/SelectScene/leftBtnP.png");
    setBtnBackground(ui->rightBtn, ":/res/SelectScene/rightBtn.png", ":/res/SelectScene/rightBtnP.png");
    setBtnBackground(ui->playBtn, ":/res/SelectScene/playBtn.png", ":/res/SelectScene/playBtnP.png");
    setBtnBackground(ui->backBtn, ":/res/SelectScene/backBtn.png", ":/res/SelectScene/backBtnP.png");
    setBtnBackground(ui->helpBtn, ":/res/SelectScene/helpBtn.png", ":/res/SelectScene/helpBtnP.png");

    // 添加地图图片
    ui->mapImg->addImage(":/res/SelectScene/map1.png");
    ui->mapImg->addImage(":/res/SelectScene/map2.png");
    ui->mapImg->addImage(":/res/SelectScene/map3.png");

    connect(ui->leftBtn, &QPushButton::clicked, [this] { ui->mapImg->previousImage(); });
    connect(ui->rightBtn, &QPushButton::clicked, [this] { ui->mapImg->nextImage(); });
    connect(ui->backBtn, SIGNAL(clicked()), this, SIGNAL(toMainPage()));
    connect(ui->playBtn, SIGNAL(clicked()), this, SIGNAL(toGamePage()));
    connect(ui->helpBtn, &QPushButton::clicked, [this] {
        QMessageBox::warning(this, "帮助", "你确定你不会玩保卫萝卜？");
    });
}

SelectWidget::~SelectWidget() { delete ui; }

Slideshow::Slideshow(QWidget *parent) : QWidget(parent) {
    // 创建两个标签用于切换
    currentLabel = new QLabel(this);
    nextLabel = new QLabel(this);

    // 设置标签属性
    currentLabel->setScaledContents(true);
    nextLabel->setScaledContents(true);

    // 初始隐藏下一个标签
    nextLabel->hide();
}

void Slideshow::addImage(const QString &path) {
    images.append(QPixmap(path));
    if (images.size() == 1) {
        currentLabel->setPixmap(images[0]);
    }
}

void Slideshow::nextImage() {
    if (images.size() < 2 || isAnimating) return;

    int nextIndex = (currentIndex + 1) % images.size();
    switchToImage(nextIndex, true);
}

void Slideshow::previousImage() {
    if (images.size() < 2 || isAnimating) return;

    int nextIndex = (currentIndex - 1 + images.size()) % images.size();
    switchToImage(nextIndex, false);
}

void Slideshow::switchToImage(int newIndex, bool forward) {
    isAnimating = true;
    nextLabel->setPixmap(images[newIndex]);
    nextLabel->show();
    nextLabel->raise();

    // 根据方向设置初始位置
    if(forward) {
        nextLabel->setGeometry(width(), 0, width(), height());  // 从右边进入
    } else {
        nextLabel->setGeometry(-width(), 0, width(), height()); // 从左边进入
    }

    // 创建动画组
    QParallelAnimationGroup* group = new QParallelAnimationGroup(this);

    // 设置动画结束位置
    QRect currentEndRect = forward ?
        QRect(-width(), 0, width(), height()) :  // 向左移出
        QRect(width(), 0, width(), height());    // 向右移出
    QRect nextEndRect(0, 0, width(), height());

    // 当前图片移出
    QPropertyAnimation* currentAnim = new QPropertyAnimation(currentLabel, "geometry", group);
    currentAnim->setDuration(300);
    currentAnim->setStartValue(currentLabel->geometry());
    currentAnim->setEndValue(currentEndRect);
    group->addAnimation(currentAnim);

    // 新图片移入
    QPropertyAnimation* nextAnim = new QPropertyAnimation(nextLabel, "geometry", group);
    nextAnim->setDuration(300);
    nextAnim->setStartValue(nextLabel->geometry());
    nextAnim->setEndValue(nextEndRect);
    group->addAnimation(nextAnim);

    // 动画完成后清理
    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        // 交换标签
        QLabel* temp = currentLabel;
        currentLabel = nextLabel;
        nextLabel = temp;

        // 重置状态
        nextLabel->hide();
        currentIndex = newIndex;
        isAnimating = false;
    });

    // 防止内存泄露
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

Slideshow::~Slideshow() {
    delete currentLabel;
    delete nextLabel;
}

int SelectWidget::mapIndex() {
    return ui->mapImg->currentIndex;
}
