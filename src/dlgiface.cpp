/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "color_tables.h"
#include "defs.h"
#include "local_str.h"
#include "staticstr.h"
#include "datefcst.h"
#include "places.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "shaperead.h"
#include "cntrs.h"
#include "sets.h"
#include "infodata.h"
#include "graphicsvw.h"
#include "winc.h"
#include "dlgiface.h"

void set_button_color(QPushButton *pb, QColor *col)
{
  QPalette palette = pb->palette();
  palette.setColor(QPalette::Button, *col);
  pb->setPalette(palette);
  QWidget::connect(pb, static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked), [=]() {
    *col = QColorDialog::getColor(*col);
    QPalette palette = pb->palette();
    palette.setColor(QPalette::Button, *col);
    pb->setPalette(palette);
  });
}

QString pal_item_name(int pal_idx)
{
  pal_idx = MIN(NCL_PAL_NUM - 1, MAX(0, pal_idx));
  QString s = QString::number(pal_idx + 1);
  s += " [";
  s += QString::number(ncl_pal_ctbl_sz[pal_idx]);
  s += " #] ";
  s += ncl_pal_names[pal_idx];
  return s;
}

dlg_iface_c::dlg_iface_c(win_c *pwin) : QDialog(pwin)
{
  m_win = pwin;
  sets_c *sets = m_win->m_sets;
  m_win->wait_cursor(true);

  QTabWidget *tab = new QTabWidget;
  {
    QFrame *tab1 = new QFrame;
    QHBoxLayout *hbox;
    {
      QVBoxLayout *vbox = new QVBoxLayout;
      tab1->setLayout(vbox);
      hbox = new QHBoxLayout;
      vbox->addLayout(hbox);
    }
    {
      QVBoxLayout *vbox1 = new QVBoxLayout;
      hbox->addLayout(vbox1);
      {
        QGroupBox *gb = new QGroupBox(IDSTR_CHAR_SET);
        vbox1->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        QComboBox *cb = new QComboBox;
        form->addRow("", cb);

        QList<QString> list;
        for (int i = 0; i < CHAR_SET_NUM; i++)
          list.append(char_set_names[i]);
        cb->addItems(list);
        cb->setCurrentIndex(sets->m_charset);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) {
          m_win->m_sets->m_charset = MAX(0, i);
          m_win->message_box(QMessageBox::Information, IDSTR_RESTART_THE_PROG, QMessageBox::Ok);
        });
      }
      {
        QGroupBox *gb = new QGroupBox;
        vbox1->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        QCheckBox *chb = new QCheckBox();
        chb->setCheckState(sets->m_proxy ? Qt::Checked : Qt::Unchecked);
        form->addRow(IDSTR_NETWORK_PROXY, chb);

        QLineEdit *proxy_host = new QLineEdit(sets->m_proxy_host);
        proxy_host->setMinimumWidth(100);
        form->addRow(IDSTR_HOST, proxy_host);

        QWidget::connect(proxy_host, static_cast<void(QLineEdit::*)()>(&QLineEdit::editingFinished), [=]() {
          m_win->m_sets->m_proxy_host = proxy_host->text();
          m_win->m_sets->m_proxy_host.trimmed();
        });

        QSpinBox *proxy_port = new QSpinBox;
        form->addRow(IDSTR_PORT, proxy_port);

        proxy_port->setRange(1, 65535);
        proxy_port->setValue(sets->m_proxy_port);

        QWidget::connect(proxy_port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) {
          m_win->m_sets->m_proxy_port = i;
        });

        QLineEdit *proxy_user = new QLineEdit(sets->m_proxy_user);
        form->addRow(IDSTR_USER, proxy_user);

        QWidget::connect(proxy_user, static_cast<void(QLineEdit::*)()>(&QLineEdit::editingFinished), [=]() {
          m_win->m_sets->m_proxy_user = proxy_user->text();
          m_win->m_sets->m_proxy_user.trimmed();
        });

        QLineEdit *proxy_passwd = new QLineEdit(sets->m_proxy_passwd);
        form->addRow(IDSTR_PASSWORD, proxy_passwd);

        QWidget::connect(proxy_passwd, static_cast<void(QLineEdit::*)()>(&QLineEdit::editingFinished), [=]() {
          m_win->m_sets->m_proxy_passwd = proxy_passwd->text();
          m_win->m_sets->m_proxy_passwd.trimmed();
        });

        QWidget::connect(chb, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [=](int state) {
          sets->m_proxy = state == Qt::Checked;
          proxy_host->setEnabled(sets->m_proxy);
          proxy_port->setEnabled(sets->m_proxy);
          proxy_user->setEnabled(sets->m_proxy);
          proxy_passwd->setEnabled(sets->m_proxy);
        });
      }
      {
        QGroupBox *gb = new QGroupBox(IDSTR_ANIMATION);
        vbox1->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        QSpinBox *sb = new QSpinBox;
        form->addRow(IDSTR_DELAY_MSEC, sb);

        sb->setRange(10, 10000);
        sb->setSingleStep(100);
        sb->setValue(sets->m_play_timeout);

        QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_play_timeout = i; });
      }
    }
    {
      QVBoxLayout *vbox2 = new QVBoxLayout;
      hbox->addLayout(vbox2);
      {
        QGroupBox *gb = new QGroupBox(IDSTR_ISOLINE_STYLE);
        vbox2->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        QFontComboBox *fcb = new QFontComboBox;
        form->addRow(IDSTR_FONT_FACE, fcb);
        {
          QFont font(sets->m_iso_font_family, sets->m_iso_font_size, sets->m_iso_font_weight);
          fcb->setCurrentFont(font);
        }
        QWidget::connect(fcb, static_cast<void(QFontComboBox::*)(const QFont &font)>(&QFontComboBox::currentFontChanged), [=](const QFont &font) {
          m_win->m_sets->m_iso_font_family = font.family();
        });

        QSpinBox *sb = new QSpinBox;
        form->addRow(IDSTR_FONT_SIZE, sb);

        sb->setRange(2, 30);
        sb->setValue(sets->m_iso_font_size);

        QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_iso_font_size = i; });

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_WEIGHT, cb);
        {
          QList<QString> list1 = { "Thin", "ExtraLight", "Light", "Normal", "Medium", "DemiBold", "Bold", "ExtraBold", "Black" };
          cb->addItems(list1);
          cb->setCurrentIndex(
            sets->m_iso_font_weight == QFont::Thin ? 0 :
            sets->m_iso_font_weight == QFont::ExtraLight ? 1 :
            sets->m_iso_font_weight == QFont::Light ? 2 :
            sets->m_iso_font_weight == QFont::Medium ? 4 :
            sets->m_iso_font_weight == QFont::DemiBold ? 5 :
            sets->m_iso_font_weight == QFont::Bold ? 6 :
            sets->m_iso_font_weight == QFont::ExtraBold ? 7 :
            sets->m_iso_font_weight == QFont::Black ? 8 : 3
          );
          QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) {
            m_win->m_sets->m_iso_font_weight =
              i == 0 ? QFont::Thin :
              i == 1 ? QFont::ExtraLight :
              i == 2 ? QFont::Light :
              i == 4 ? QFont::Medium :
              i == 5 ? QFont::DemiBold :
              i == 6 ? QFont::Bold :
              i == 7 ? QFont::ExtraBold :
              i == 8 ? QFont::Black : 3;
          });
        }
        QPushButton *pb = new QPushButton;
        form->addRow(IDSTR_COLOR, pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_iso_color);

        QDoubleSpinBox *dsb = new QDoubleSpinBox;
        form->addRow(IDSTR_WIDTH, dsb);

        dsb->setRange(0.1, 30);
        dsb->setSingleStep(0.1);
        dsb->setValue(sets->m_iso_width);
        QWidget::connect(dsb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) { m_win->m_sets->m_iso_width = d; });

        sb = new QSpinBox;
        form->addRow(IDSTR_STEP, sb);

        sb->setRange(1, 15);
        sb->setValue(sets->m_iso_step);
        QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_iso_step = i; });

        QCheckBox *chb = new QCheckBox;
        form->addRow(IDSTR_ERASE_LABEL_BKG, chb);

        chb->setChecked(sets->m_iso_lab_bgerase ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(chb, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [=](int i) {
          m_win->m_sets->m_iso_lab_bgerase = (i == Qt::Checked); });

        QHBoxLayout *hbox1 = new QHBoxLayout;
        form->addRow(IDSTR_SMOOTH, hbox1);

        chb = new QCheckBox;
        hbox1->addWidget(chb);

        chb->setChecked(sets->m_iso_smooth ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(chb, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [=](int i) {
          m_win->m_sets->m_iso_smooth = (i == Qt::Checked); });

        QLabel *lb = new QLabel(IDSTR_SMOOTH_VALUE);
        hbox1->addWidget(lb);

        sb = new QSpinBox;
        hbox1->addWidget(sb);

        sb->setRange(3, 300);
        sb->setValue(sets->m_iso_smooth_val);

        QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_iso_smooth_val = i; });

        sb = new QSpinBox;
        form->addRow(IDSTR_LABEL_DENSITY, sb);

        sb->setRange(1, 1000);
        sb->setSingleStep(10);
        sb->setValue(sets->m_iso_label_density);
        QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_iso_label_density = i; });
      }
      {
        QGroupBox *gb = new QGroupBox(IDSTR_GEOGRAPHIC_GRID);
        vbox2->addWidget(gb);

        QHBoxLayout *hbox = new QHBoxLayout;
        gb->setLayout(hbox);

        hbox->addWidget(new QLabel(IDSTR_COLOR));

        QPushButton *pb = new QPushButton;
        hbox->addWidget(pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_geogrid_color);

        hbox->addWidget(new QLabel(IDSTR_WIDTH));

        QDoubleSpinBox *dsb = new QDoubleSpinBox;
        hbox->addWidget(dsb);

        dsb->setRange(1, 10);
        dsb->setSingleStep(0.2);
        dsb->setValue(sets->m_geogrid_width);
        QWidget::connect(dsb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) { m_win->m_sets->m_geogrid_width = d; });

        QCheckBox *chb = new QCheckBox(IDSTR_LABEL);
        hbox->addWidget(chb);

        chb->setChecked(sets->m_geogrid_label ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(chb, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [=](int i) {
          m_win->m_sets->m_geogrid_label = (i == Qt::Checked); });
      }
    }
    {
      QVBoxLayout *vbox3 = new QVBoxLayout;
      hbox->addLayout(vbox3);
      {
        QGroupBox *gb = new QGroupBox(IDSTR_ISOLINE_WIND);
        vbox3->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        gb->setCheckable(true);
        gb->setChecked(sets->m_wind_compose ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(gb, static_cast<void(QGroupBox::*)(bool)>(&QGroupBox::toggled), [=](bool f) {m_win->m_sets->m_wind_compose = f; });

        QDoubleSpinBox *dsb = new QDoubleSpinBox;
        form->addRow(IDSTR_ARROW_SIZE, dsb);

        dsb->setRange(0.1, 10);
        dsb->setSingleStep(0.2);
        dsb->setValue(sets->m_wind_arrow_size);
        QWidget::connect(dsb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) { m_win->m_sets->m_wind_arrow_size = d; });

        dsb = new QDoubleSpinBox;
        form->addRow(IDSTR_ARROW_WIDTH, dsb);

        dsb->setRange(1, 10);
        dsb->setSingleStep(0.2);
        dsb->setValue(sets->m_wind_arrow_width);
        QWidget::connect(dsb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double d) { m_win->m_sets->m_wind_arrow_width = d; });

        QPushButton *pb = new QPushButton;
        form->addRow(IDSTR_COLOR, pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_wind_color);

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_HPA_LEVEL, cb);

        int i;
        for (i = 0; i < TAG_TO_LEVEL_NUM; i++)
          if (gfs_tag2level[i].tag == string("UGRD") && gfs_tag2level[i].lev == string("ISBL"))
            break;
        int j = 0;
        while (gfs_tag2level[i].lval[j]) {
          cb->addItem(gfs_tag2level[i].lval[j]);
          if (sets->m_wind_lval == gfs_tag2level[i].lval[j])
            cb->setCurrentIndex(j);
          j++;
        }
        QWidget::connect(cb, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &lval) {
          m_win->m_sets->m_wind_lval = lval; });
      }
      {
        QGroupBox *gb = new QGroupBox(IDSTR_ISOLINE_TEMPERATURE);
        vbox3->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        gb->setCheckable(true);
        gb->setChecked(sets->m_tempre_compose ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(gb, static_cast<void(QGroupBox::*)(bool)>(&QGroupBox::toggled), [=](bool f) { m_win->m_sets->m_tempre_compose = f; });

        QPushButton *pb = new QPushButton;
        form->addRow(IDSTR_COLOR, pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_tempre_color);

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_LEVEL, cb);

        int i;
        for (i = 0; i < TAG_TO_LEVEL_NUM; i++)
          if (gfs_tag2level[i].tag == string("TMP") && gfs_tag2level[i].lev == string("ISBL"))
            break;
        int j = 0;
        while (gfs_tag2level[i].lval[j]) {
          cb->addItem(gfs_tag2level[i].lval[j]);
          if (sets->m_tempre_lval == gfs_tag2level[i].lval[j])
            cb->setCurrentIndex(j);
          j++;
        }
        QWidget::connect(cb, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &lval) {
          m_win->m_sets->m_tempre_lval = lval; });
      }
      {
        QGroupBox *gb = new QGroupBox(IDSTR_ISOLINE_PRESSURE);
        vbox3->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        gb->setCheckable(true);
        gb->setChecked(sets->m_geopt_compose ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(gb, static_cast<void(QGroupBox::*)(bool)>(&QGroupBox::toggled), [=](bool f) { m_win->m_sets->m_geopt_compose = f; });

        QPushButton *pb = new QPushButton;
        form->addRow(IDSTR_COLOR, pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_geopt_color);

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_LEVEL, cb);

        int i;
        for (i = 0; i < TAG_TO_LEVEL_NUM; i++)
          if (gfs_tag2level[i].tag == string("HGT") && gfs_tag2level[i].lev == string("ISBL"))
            break;
        int j = 0;
        while (gfs_tag2level[i].lval[j]) {
          cb->addItem(gfs_tag2level[i].lval[j]);
          if (sets->m_geopt_lval == gfs_tag2level[i].lval[j])
            cb->setCurrentIndex(j);
          j++;
        }
        QWidget::connect(cb, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &lval) {
          m_win->m_sets->m_geopt_lval = lval; });
      }
    }
    {
      QVBoxLayout *vbox4 = new QVBoxLayout;
      hbox->addLayout(vbox4);
      {
        QGroupBox *gb = new QGroupBox(IDSTR_PRECIPITATION);
        vbox4->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        gb->setCheckable(true);
        gb->setChecked(sets->m_precip_compose ? Qt::Checked : Qt::Unchecked);
        QWidget::connect(gb, static_cast<void(QGroupBox::*)(bool)>(&QGroupBox::toggled), [=](bool f) { m_win->m_sets->m_precip_compose = f; });

        QPushButton *pb = new QPushButton;
        form->addRow(IDSTR_COLOR, pb);

        pb->setFlat(true);
        pb->setAutoFillBackground(true);

        set_button_color(pb, &sets->m_precip_color);

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_CODE, cb);
        cb->addItem("CWAT");

        QWidget::connect(cb, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &lval) {
          m_win->m_sets->m_precip_tag = lval; });
      }
      {
        QGroupBox *gb = new QGroupBox(IDSTR_UNITS);
        vbox4->addWidget(gb);

        QFormLayout *form = new QFormLayout;
        gb->setLayout(form);

        QComboBox *cb = new QComboBox;
        form->addRow(IDSTR_TEMPERATURE, cb);

        cb->addItem(TEMPERATURE_LABEL(0));
        cb->addItem(TEMPERATURE_LABEL(1));
        cb->addItem(TEMPERATURE_LABEL(2));
        cb->setCurrentIndex(sets->m_units_temp);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_temp = i; });

        cb = new QComboBox;
        form->addRow(IDSTR_GEOPOTENTIAL, cb);

        for (int i = 0; i < 4; i++)
          cb->addItem(GEOPOTENTIAL_LABEL(i));
        cb->setCurrentIndex(sets->m_units_geopt);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_geopt = i; });

        QComboBox *cb3 = new QComboBox;
        form->addRow(IDSTR_SPEED, cb3);

        for (int i = 0; i < 5; i++)
          cb3->addItem(SPEED_LABEL(i));
        cb3->setCurrentIndex(sets->m_units_speed);
        QWidget::connect(cb3, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_speed = i; });

        cb = new QComboBox;
        form->addRow(IDSTR_PRECIPITATION, cb);

        cb->addItem(PRECIPITATION_LABEL(0));
        cb->addItem(PRECIPITATION_LABEL(1));
        cb->setCurrentIndex(sets->m_units_precip);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_precip = i; });

        cb = new QComboBox;
        form->addRow(IDSTR_PRECIP_RATE, cb);

        cb->addItem(PRECIP_RATE_LABEL(0));
        cb->addItem(PRECIP_RATE_LABEL(1));
        cb->addItem(PRECIP_RATE_LABEL(2));
        cb->setCurrentIndex(sets->m_units_prerate);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_prerate = i; });

        cb = new QComboBox;
        form->addRow(IDSTR_PRESSURE, cb);

        for (int i = 0; i < 4; i++)
          cb->addItem(PRESSURE_LABEL(i));
        cb->setCurrentIndex(sets->m_units_press);
        QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int i) { m_win->m_sets->m_units_press = i; });
      }
    }
    tab->addTab(tab1, IDSTR_INTERFACE);
  }
  {
    QStandardItemModel *model = new QStandardItemModel;
    QStringList header_sl1({ IDSTR_ABBREV,  IDSTR_NaME, IDSTR_PALETTE });
    model->setColumnCount(header_sl1.size());
    model->setHorizontalHeaderLabels(header_sl1);

    QStandardItem *root = model->invisibleRootItem();

    string tag;
    for (int i = 0; i < TAG_TO_LEVEL_NUM; i++) {
      if (tag == gfs_tag2level[i].tag)
        continue;
      tag = gfs_tag2level[i].tag;
      QList<QStandardItem*> tmp;
      QStandardItem *item = new QStandardItem(gfs_tag2level[i].tag);
      item->setEditable(false);
      tmp.append(item);
      item = new QStandardItem(grib2_all[gfs_tag2level[i].idx].short_name);
      item->setEditable(false);
      tmp.append(item);
      int j = sets->m_pal_idx[GFS_025][tag];
      QString s = pal_item_name(j);
      s += "                                                          ";
      item = new QStandardItem(s);
      item->setData(QVariant(QPixmap(ncl_pal_xpm[j])), Qt::DecorationRole);
      role_tag_c par(GFS_025, tag);
      item->setData(QVariant::fromValue<role_tag_c>(par));
      tmp.append(item);

      root->appendRow(tmp);
    }
    QTableView *view = new QTableView;
    view->setAlternatingRowColors(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    view->setModel(model);
    palette_item_delegate_c *delegate = new palette_item_delegate_c(m_win, this);
    view->setItemDelegate(delegate);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->resizeColumnToContents(2);
    view->resizeColumnToContents(3);
    {
      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(view);

      QFrame *tab2 = new QFrame;
      tab2->setLayout(vbox);

      tab->addTab(tab2, IDSTR_GRIB_PALETTES);
    }

    QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(tab);
    vbox->addWidget(bbox);
    setLayout(vbox);

    connect(bbox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(accept()));
    connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
  }

  setWindowTitle(IDSTR_SETTINGS);
  m_win->wait_cursor(false);
  resize(800, 400);
}

void dlg_iface_c::accept()
{
  bool f = m_win->m_sets->save_ini(SETS_GROUP_INTERFACE, nullptr);
  bool f1 = true;

  m_win->set_msg(f && f1 ? IDSTR_OP_SUCCEEDED : IDSTR_OP_FAILED, false, true);
  QDialog::accept();
}

QWidget* palette_item_delegate_c::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QWidget *editor = (QWidget *) new QComboBox(parent);
  QComboBox *cb = (QComboBox *)editor;
  for (int i = 0; i < NCL_PAL_NUM; i++) {
    QString s = pal_item_name(i);
    cb->addItem(QPixmap(ncl_pal_xpm[i]), s);
  }
  cb->setIconSize(QSize(150, 16));
  return editor;
}

void palette_item_delegate_c::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  sets_c *sets = m_win->m_sets;
  if (!index.isValid() || !index.data(Qt::UserRole + 1).isValid())
    return;
  role_tag_c obj = index.data(Qt::UserRole + 1).value<role_tag_c>();

  QComboBox *cb = qobject_cast<QComboBox *>(editor);
  if (cb) {
    cb->setCurrentIndex(sets->m_pal_idx[obj.m_role][obj.m_tag]);
  }
}

void palette_item_delegate_c::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  sets_c *sets = m_win->m_sets;
  if (!index.isValid() || !index.data(Qt::UserRole + 1).isValid())
    return;
  role_tag_c obj = index.data(Qt::UserRole + 1).value<role_tag_c>();

  QComboBox *cb = qobject_cast<QComboBox *>(editor);
  if (cb) {
    int i = cb->currentIndex();
    sets->m_pal_idx[obj.m_role][obj.m_tag] = i;
    model->setData(index, QVariant(QPixmap(ncl_pal_xpm[i])), Qt::DecorationRole);
    QString s = pal_item_name(i);
    model->setData(index, s, Qt::DisplayRole);
  }
}

void palette_item_delegate_c::commit_and_close()
{
  if (QWidget *pb = qobject_cast<QWidget *>(sender())) {
    emit commitData(pb);
    emit closeEditor(pb);
  }
}
