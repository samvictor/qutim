#include "plugmansettings.h"
#include "plugxmlhandler.h"
#include <QListWidgetItem>
#include "pluginstaller.h"
plugmanSettings::plugmanSettings(const QString &profile_name)
: m_package_model(new plugPackageModel)
{
	setupUi(this);
	m_profile_name = profile_name;
// 	connect(installfromfileBtn, SIGNAL(clicked()), this,  SIGNAL(installfromfileclick()));
	progressBar->setVisible(false);
	treeView->setModel(m_package_model);
	getPackageList();
	treeView->expandAll();
}

plugmanSettings::~plugmanSettings()
{
	delete(m_package_model);
}

void plugmanSettings::getPackageList() {
	plugXMLHandler plug_handler;
	m_package_model->clear();
	QHash<quint16, packageInfo> package_list= plug_handler.getPackageList();
	QHash<quint16, packageInfo>::iterator it = package_list.begin();
	for (;it != package_list.end();it++) {
		ItemData item(buddy,QIcon(":/icons/hi64-action-package.png"),package_list.value(it.key()),it.key());
		m_package_model->addItem(item,item.packageItem.properties.value("name"));
	}
	return;
}

void plugmanSettings::on_installfromfileBtn_clicked() {
	plugInstaller *plug_install = new plugInstaller;
	plug_install->setParent(this);
	plug_install->setProgressBar(progressBar);
// 	progressBar->setFormat(tr("Install package : %p%"));
	plug_install->installPackage();
// 	connect(plug_install,SIGNAL(finished()),this,SLOT(updatePackageList()));
	connect(plug_install,SIGNAL(destroyed(QObject*)),this,SLOT(updatePackageList()));
}

void plugmanSettings::updatePackageList() {
	getPackageList();
 	progressBar->setVisible(false);
}
