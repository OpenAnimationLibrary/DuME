#ifndef BLOCKVIDEOSPEED_H
#define BLOCKVIDEOSPEED_H

#include "ui_blockvideospeed.h"
#include "UI/Blocks/blockcontentwidget.h"

class BlockVideoSpeed : public BlockContentWidget, private Ui::BlockVideoSpeed
{
    Q_OBJECT

public:
    explicit BlockVideoSpeed(MediaInfo *mediaInfo, QWidget *parent = nullptr);
public slots:
    void activate( bool blockEnabled );
    void update();
private slots:
    void on_speedBox_valueChanged(double arg1);
private:
    void updateInterpolationUI();
};

#endif // BLOCKVIDEOSPEED_H
