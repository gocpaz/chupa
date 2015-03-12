#include "AzZoomSlider.h"


#include <math.h>
#include <QtMath>
#include <QDebug>

AzZoomSlider::AzZoomSlider(QWidget *parent) : QWidget(parent) {
    mIsSliderChangedValue = false;

    mSpinBox = new QDoubleSpinBox(this);
    mSpinBox->setKeyboardTracking(false);
    mSlider = new QSlider(this);
    mSlider->setOrientation(Qt::Horizontal);
    mSlider->setMinimum(1);

    mLayout = new QHBoxLayout(this);
    mLayout->addWidget(mSlider);
    mLayout->addWidget(mSpinBox);
    setLayout(mLayout);
    mLayout->setMargin(0);

    setMaximumScale(4);
    //setValue(2);

    connect(mSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged()));
    connect(mSpinBox,SIGNAL(valueChanged(double)),this,SLOT(spinBoxChanged()));
}

AzZoomSlider::~AzZoomSlider() {
}

void AzZoomSlider::setValue(qreal scale) {
    if (scale == mCurrentScale)
        return;
    mCurrentScale = scale;

    if (!mIsSliderChangedValue)
        mSlider->setValue(scaleToSlider(scale));
    mSpinBox->setValue(scale*100);

    emit scaleChanged(scale);

}

void AzZoomSlider::setPercentValue(qreal scale) {
    setValue(scale / 100.0);
}

/*!
 * \brief AzZoomSlider::scaleFromSlider
 * \return scale from slider position, where pos - slider position
 */
qreal AzZoomSlider::scaleFromSlider(int pos) const {
    return qPow(2.0,(pos-mSlider->maximum()/2)/100.0);
}

qreal AzZoomSlider::scaleFromSlider() const {
    return scaleFromSlider(mSlider->value());
}

int AzZoomSlider::scaleToSlider(qreal scale) const {
    return log2(scale) * 100 + mSlider->maximum()/2;
}

/*!
 * \brief AzZoomSlider::setMaximumScale
 * \param scale
 * Set maximum scale
 * \Warning - reset scale value to 1
 */

void AzZoomSlider::setMaximumScale(qreal scale) {
    if (scale == mMaximumScale)
        return;
    mMaximumScale = scale;
    int sliderMax = log2(mMaximumScale) * 200;
    mSlider->setMaximum(sliderMax);
    mSpinBox->setMaximum(mMaximumScale*100);
    mSpinBox->setMinimum(scaleFromSlider(mSlider->minimum())*100);

     //init by hand
    setValue(1);
    mSpinBox->setValue(1);
    mSlider->setValue(mSlider->maximum()/2);
    if (mCurrentScale != 1) {
        mCurrentScale = 1;
        emit scaleChanged(1);
    }
}

/*!
 * \brief AzZoomSlider::sliderChanged
 * scale change slider
 */
void AzZoomSlider::sliderChanged() {
    mIsSliderChangedValue = true;
    //qDebug() << "Slider change" << scaleFromSlider();
    setValue(scaleFromSlider());
    mIsSliderChangedValue = false;

}

void AzZoomSlider::spinBoxChanged() {
    if (mSpinBox->value() != mCurrentScale / 100) {
        //qDebug() << "SpinBox change" << mSpinBox->value()/100;
        setValue(mSpinBox->value()/100);
    }
}

