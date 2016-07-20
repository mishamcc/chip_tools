#ifndef CHIP_TOOLS_SETTINGS_SAVER_HPP_
#define CHIP_TOOLS_SETTINGS_SAVER_HPP_

#include <QObject>
#include <QString>
#include <QSettings>
#include <QList>

#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <QComboBox>

namespace chip_tools
{

namespace sdk
{

template<class Base, class Widget>
struct SettingsSaver
{
   static void save_all(const QObject &object)
   {
      QSettings settings;
      settings.beginGroup(object.objectName());
      QList<Widget *> children = object.findChildren<Widget *>();
      foreach (const Widget *child, children)
         Base::save(settings, child);

      settings.endGroup();
   }

   static void load_all(QObject &object)
   {
      QSettings settings;
      settings.beginGroup(object.objectName());

      QList<Widget *> children = object.findChildren<Widget *>();
      foreach (Widget *child, children)
         Base::load(settings, child);

      settings.endGroup();
   }
}; // class SettingsSaver

struct QLineEditSaver : public SettingsSaver<QLineEditSaver, QLineEdit>
{
   static void load(QSettings &settings, QLineEdit *widget)
   {
      widget->setText(settings.value(widget->objectName(), "").toString());
   }

   static void save(QSettings &settings, const QLineEdit *widget)
   {
      settings.setValue(widget->objectName(), widget->text());
   }
}; // class QLineEditSaver

struct QSpinBoxSaver : public SettingsSaver<QSpinBoxSaver, QSpinBox>
{
   static void load(QSettings &settings, QSpinBox *widget)
   {
      widget->setValue(settings.value(widget->objectName(), 0).toInt());
   }

   static void save(QSettings &settings, const QSpinBox *widget)
   {
      settings.setValue(widget->objectName(), widget->value());
   }
}; // class QSpinBoxSaver

struct QDoubleSpinBoxSaver : public SettingsSaver<QDoubleSpinBoxSaver, QDoubleSpinBox>
{
   static void load(QSettings &settings, QDoubleSpinBox *widget)
   {
      widget->setValue(settings.value(widget->objectName(), 0).toDouble());
   }

   static void save(QSettings &settings, const QDoubleSpinBox *widget)
   {
      settings.setValue(widget->objectName(), widget->value());
   }
}; // class QDoubleSpinBoxSaver

struct QButtonSaver : public SettingsSaver<QButtonSaver, QCheckBox>
{
   static void load(QSettings &settings, QCheckBox *widget)
   {
      widget->setChecked(settings.value(widget->objectName(), true).toBool());
   }

   static void save(QSettings &settings, const QCheckBox *widget)
   {
      settings.setValue(widget->objectName(), widget->isChecked());
   }
}; // class QButtonSaver

struct QTabWidgetSaver : public SettingsSaver<QTabWidgetSaver, QTabWidget>
{
   static void load(QSettings &settings, QTabWidget *widget)
   {
      widget->setCurrentIndex(settings.value(widget->objectName(), true).toInt());
   }

   static void save(QSettings &settings, const QTabWidget *widget)
   {
      settings.setValue(widget->objectName(), widget->currentIndex());
   }
}; // class QTabWidgetSaver

struct QComboBoxSaver : public SettingsSaver<QComboBoxSaver, QComboBox>
{
   static void load(QSettings &settings, QComboBox *widget)
   {
      widget->setCurrentIndex(settings.value(widget->objectName(), true).toInt());
   }

   static void save(QSettings &settings, const QComboBox *widget)
   {
      settings.setValue(widget->objectName(), widget->currentIndex());
   }
}; // class QComboBoxSaver

} // namespace sdk

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_SETTINGS_SAVER_HPP_
 
