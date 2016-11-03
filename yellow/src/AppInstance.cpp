#include "AppInstance.h"

#include <QFontDatabase>
#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QDirIterator>
#include <QDir>

#include "meta.h"


namespace App {
 
    
//    class DataObject : public QObject
//    {
//        Q_OBJECT
//        
//        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//        Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
//        
//    };

    
    void Instance::init() {
        QApplication::setOrganizationName(Meta::orgName);
        QApplication::setOrganizationDomain(Meta::orgDomain);
        QApplication::setApplicationName(Meta::appName);
        QApplication::setApplicationVersion(Meta::versionString);

        QString resourceDir = Meta::ressourceDirectory;

        AddFontDir(resourceDir + "/fonts/");

        QString we_are = "KOUKOU - Super Projet";
        
        _view.reset(new QQuickView());
        _view->setTitle(QGuiApplication::applicationName());

        auto engine = _view->engine();

        engine->addImportPath(resourceDir + "/qml");

        _view->setSource(QUrl::fromLocalFile(resourceDir + "/qml/App/window.qml"));
    
        _view->setResizeMode(QQuickView::SizeRootObjectToView);
    
        
        QQuickItem *object = _view->QQuickView::rootObject();
        
        
//        QList<QObject*> dataList;
//        dataList.append(new DataObject("Item 1", "red"));
//        dataList.append(new DataObject("Item 2", "green"));
//        dataList.append(new DataObject("Item 3", "blue"));
//        dataList.append(new DataObject("Item 4", "yellow"));
//        
//        QQuickView view;
//        view.setResizeMode(QQuickView::SizeRootObjectToView);
        
        
        
        
        // http://doc.qt.io/qt-5/qml-qtquick-listview.html
        
        //
        
        //passing custom object
        ///// setup data list
        QStringList dataList;
        dataList.append("Simple listview");
        dataList.append("declared in appInstance.cpp");
        dataList.append("avec des lignes");
        dataList.append("etc.");
        
        object->setProperty("myModel", QVariant::fromValue(dataList));

        
        /////
        
        object->setProperty("text2Text",QVariant(we_are));

        
        
        _view->show();
        
    }

    int Instance::run(int argc, char** argv) {
        QApplication application(argc, argv);

        init();
        return application.exec();
    }

    void Instance::AddFontDir(QString const& directory) {
        QDirIterator it(directory, {"*.ttf", "*.otf"}, QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QFontDatabase::addApplicationFont(it.next());
        }
    }
}
