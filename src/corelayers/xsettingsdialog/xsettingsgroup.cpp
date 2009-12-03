/****************************************************************************
 *  xsettingsgroup.cpp
 *
 *  Copyright (c) 2009 by Sidorov Aleksey <sauron@citadelspb.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "xsettingsgroup.h"
#include "ui_xsettingsgroup.h"
#include <libqutim/settingswidget.h>
#include <QDebug>

XSettingsGroup::XSettingsGroup ( const qutim_sdk_0_3::SettingsItemList& settings, QWidget* parent )
: QWidget (parent ), ui (new Ui::XSettingsGroup)
{
	m_setting_list = settings;
	ui->setupUi(this);

	foreach (SettingsItem *settings_item, m_setting_list)
	{
		QListWidgetItem *list_item = new QListWidgetItem (settings_item->icon(),
														  settings_item->text(),
														  ui->listWidget
														  );
		//list_item->setToolTip(settings_item->description()); //TODO need short description!
	}
	connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(currentRowChanged(int)));
	currentRowChanged(0);
	qDebug() << "create group";
}


void XSettingsGroup::currentRowChanged ( int index)
{
	SettingsWidget *widget = m_setting_list.at(index)->widget();
	if (widget == 0)
		return;
	if (ui->stackedWidget->indexOf(widget) == -1)
	{
		widget->load();
		ui->stackedWidget->addWidget(widget);
		connect(widget,SIGNAL(modifiedChanged(bool)),SLOT(onWidgetModifiedChanged(bool)));
	}
	ui->stackedWidget->setCurrentWidget(widget);
	emit titleChanged(m_setting_list.at(index)->text());
}


XSettingsGroup::~XSettingsGroup()
{

}

void XSettingsGroup::onWidgetModifiedChanged(bool haveChanges)
{
	SettingsWidget *widget = qobject_cast< SettingsWidget* >(sender());
	if (!widget)
		return;
	if (haveChanges)
		emit modifiedChanged(widget);
}

