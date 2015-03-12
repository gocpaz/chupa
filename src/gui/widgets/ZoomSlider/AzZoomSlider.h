#ifndef AZZOOMSLIDER_H
#define AZZOOMSLIDER_H

#include <QtDesigner/QDesignerExportWidget>
#include <QWidget>
#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QLayout>

class QDESIGNER_WIDGET_EXPORT AzZoomSlider : public QWidget

{
    Q_OBJECT
    Q_PROPERTY(qreal maximumScale READ maximumScale WRITE setMaximumScale)
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int pageStep READ pageStep WRITE setPageStep)
    Q_PROPERTY(bool tracking READ hasTracking WRITE setTracking)
    Q_PROPERTY(QSlider::TickPosition tickPosition READ tickPosition WRITE setTickPosition)
    Q_PROPERTY(int tickInterval READ tickInterval WRITE setTickInterval)
    Q_PROPERTY(QAbstractSpinBox::ButtonSymbols buttonSymbols READ buttonSymbols WRITE setButtonSymbols)
    Q_PROPERTY(bool keyboardTracking READ keyboardTracking WRITE setKeyboardTracking)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(int desimals READ desimals WRITE setDesimals)
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY scaleChanged)
public:
    explicit AzZoomSlider(QWidget *parent = 0);
    ~AzZoomSlider();

    void setMaximumScale(qreal);

    inline int maximumPercentScale() const                  { return mMaximumScale * 100; }
    inline qreal maximumScale() const                       { return mMaximumScale; }

    inline qreal minumumPercentScale() const                { return 1/mMaximumScale;   }
    inline qreal minimumScale() const                       { return minumumPercentScale() / 100.0; }

    inline qreal value() const                              { return mCurrentScale; }
    inline qreal percentValue() const                       { return mCurrentScale * 100.0; }

    //slider & spin property (maybe separate)
    inline int singleStep() const                           { return mSlider->singleStep();   }
    void setSingleStep(int step)                            { mSlider->setSingleStep(step);mSpinBox->setSingleStep(step); }

    //slider property
    inline int pageStep() const                             { return mSlider->pageStep();     }
    inline void setPageStep(int step)                       { mSlider->setPageStep(step);     }

    inline bool hasTracking() const                         { return mSlider->hasTracking();             }
    inline void setTracking(bool enable)                    { mSlider->setTracking(enable);     }

    inline QSlider::TickPosition tickPosition() const       { return mSlider->tickPosition();   }
    inline void setTickPosition(QSlider::TickPosition pos)  { mSlider->setTickPosition(pos);    }

    inline int tickInterval() const                         { return mSlider->tickInterval();          }
    inline void setTickInterval(int interval)               { mSlider->setTickInterval(interval); }

    //spin box property
    inline QAbstractSpinBox::ButtonSymbols buttonSymbols() const            { return mSpinBox->buttonSymbols(); }
    inline void setButtonSymbols(QAbstractSpinBox::ButtonSymbols button)    { mSpinBox->setButtonSymbols(button); }

    inline bool keyboardTracking() const                                    { return mSpinBox->keyboardTracking(); }
    inline void setKeyboardTracking(bool track)                             { mSpinBox->setKeyboardTracking(track); }

    inline QString prefix() const                           { return mSpinBox->prefix(); }
    inline void setPrefix(const QString& prefix)            { mSpinBox->setPrefix(prefix); }

    inline QString suffix() const                           { return mSpinBox->suffix();           }
    inline void setSuffix(const QString& suffix)            { mSpinBox->setSuffix(suffix);      }

    inline int desimals() const                             { return mSpinBox->decimals();      }
    inline void setDesimals(int decimals)                   { mSpinBox->setDecimals(decimals);  }


public slots:
    void setValue(qreal);
    void setPercentValue(qreal);
signals:
    void scaleChanged(qreal);
private slots:
    void sliderChanged();
    void spinBoxChanged();
private:
    Q_DISABLE_COPY(AzZoomSlider)

    qreal scaleFromSlider(int) const;
    qreal scaleFromSlider() const;
    int scaleToSlider(qreal) const;
    QDoubleSpinBox *mSpinBox;
    QSlider *mSlider;
    QHBoxLayout *mLayout;
    qreal mMaximumScale;
    qreal mCurrentScale;
    bool mIsSliderChangedValue; /*! if scale changed slider (for recurse block - lost precession)*/
};

#endif // AZZOOMSLIDER_H
