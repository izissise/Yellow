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
        
        ///// setup data list
        QStringList *qlist = new QStringList();
        
        qlist->append(("Item 1"));
        qlist->append(("Item 2"));
        qlist->append(("Item 3"));
        qlist->append(("Item 4"));
        qlist->append(("Item 5"));
        
        object->setProperty("DataModel", QVariant::fromValue(qlist));
        
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
