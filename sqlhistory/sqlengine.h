/*
	SqlEngine

	Copyright (c) 2009 by Alexander Kazarin <boiler@co.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef SQLENGINE_H
#define SQLNENGINE_H

#include <qutim/plugininterface.h>
#include <qutim/layerinterface.h>
#include <qutim/layerscity.h>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

using namespace qutim_sdk_0_2;

namespace SqlHistoryNamespace {

class HistoryWindow;
class SqlHistorySettings;

class SqlEngine : public HistoryLayerInterface
{
public:
	SqlEngine();
	virtual bool init(PluginSystemInterface *plugin_system);
	virtual void release();
	virtual void setProfileName(const QString &profile_name);
	void loadSettings();
	virtual void setLayerInterface( LayerType type, LayerInterface *layer_interface);
	virtual void saveLayerSettings();
	virtual QList<SettingsStructure> getLayerSettingsList();
	virtual void removeLayerSettings();
	virtual void saveGuiSettingsPressed() {}
	virtual void removeGuiLayerSettings() {}
	virtual void openWindow(const TreeModelItem &item);
	uint findEnd(QFile &file);
	virtual bool storeMessage(const HistoryItem &item);
	virtual QList<HistoryItem> getMessages(const TreeModelItem &item, const QDateTime &last_time);
	QString getFileName(const HistoryItem &item) const;
	QDir getAccountDir(const TreeModelItem &item) const;
	static QString quote(const QString &str);
	static QString unquote(const QString &str);
	void setHistory(const TreeModelItem &item);
  inline const QString &getHistoryPath() const { return NULL; }
private:
	QHash<QString, QString> m_history_hash;
	QHash<QString, QPointer<HistoryWindow> > m_history_windows;
	SqlHistorySettings *m_settings_widget;
	QTreeWidgetItem *m_settings_item;
	bool m_save_history;
	bool m_show_recent_messages;
	quint16 m_max_num;
	QSqlDatabase db;
};

}

#endif // SQLENGINE_H
