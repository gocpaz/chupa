#ifndef AZZOOMSLIDERPLUGIN_H
#define AZZOOMSLIDERPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class AzZoomSliderPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "AzZoomSlider.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    AzZoomSliderPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool mInit;
};


#endif //AZZOOMSLIDERPLUGIN_H
