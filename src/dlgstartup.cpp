/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <algorithm>
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "defs.h"
#include "local_str.h"
#include "staticstr.h"
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "infocntrs.h"
#include "infodata.h"
#include "sets.h"
#include "winc.h"
#include "dlgstartup.h"
#include "regui.h"

dlg_startup_c::dlg_startup_c(win_c *pwin) : QDialog(pwin), m_win(pwin), m_tag_tree(pwin, false)
{
  info_data_c *data = m_win->m_data;

  QVBoxLayout *vbox1 = new QVBoxLayout;

  if (!data->gdata_empty())
    m_tag_tree.tree_from_gribs(data->gribs());

  vbox1->addWidget(m_tag_tree.m_view);

  QVBoxLayout *vbox0 = new QVBoxLayout;
  setLayout(vbox0);
  QSplitter *splitter = new QSplitter(Qt::Vertical);
  vbox0->addWidget(splitter);

  QFrame *frame1 = new QFrame;
  QFrame *frame2 = new QFrame;

  QVBoxLayout *vbox2 = new QVBoxLayout;
  frame1->setLayout(vbox1);
  frame2->setLayout(vbox2);
  splitter->addWidget(frame1);
  splitter->addWidget(frame2);
  {
    splitter->setChildrenCollapsible(false);
    QVBoxLayout *lay = new QVBoxLayout(splitter->handle(1));
    lay->setSpacing(0);
    lay->setMargin(0);
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lay->addWidget(line);
  }

  QLabel *lab1 = new QLabel(IDSTR_CONTOURS);
  vbox2->addWidget(lab1);

  int btn_h;
  {
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setSpacing(6);

    hbox->addStretch();

    QPushButton *add = new QPushButton(QIcon(QPixmap(":/icons/add3.png")), QString());
    btn_h = add->sizeHint().height();
    add->setToolTip(IDSTR_ADD_LAYER_S);
    add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hbox->addWidget(add);
    hbox->setAlignment(add, Qt::AlignRight);
    connect(add, SIGNAL(clicked()), this, SLOT(add_grib_clicked()));

    QPushButton *rem_all = new QPushButton(QIcon(QPixmap(":/icons/sc_deleteallannotation.png")), QString());
    rem_all->setToolTip(IDSTR_CLOSE_ALL);
    rem_all->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hbox->addWidget(rem_all);
    hbox->setAlignment(rem_all, Qt::AlignRight);
    connect(rem_all, SIGNAL(clicked()), this, SLOT(rem_all_clicked()));

    vbox1->addLayout(hbox);
  }

  m_model_cntrs = new QStandardItemModel;
  QStringList header_sl1({ IDSTR_FILE_NAME,  IDSTR_DBF_PARAMS, IDSTR_TEXT_COLOR, IDSTR_LINECOLOR, IDSTR_LINEWIDTH, IDSTR_DASHED,
                           IDSTR_FONT_FAMILY, IDSTR_SIZE_BRA });//, IDSTR_POINTS 
  m_model_cntrs->setColumnCount(header_sl1.size());
  m_model_cntrs->setHorizontalHeaderLabels(header_sl1);
  if (data->vec()) {
    QStandardItem *root = m_model_cntrs->invisibleRootItem();
    for_each(data->vec()->begin(), data->vec()->end(), [&](info_cntrs_c *info) {
      root->appendRow(cntrs_add_row(info));
    });
  }
  m_view_cntrs = new QTableView(this);
  m_view_cntrs->setAlternatingRowColors(true);
  m_view_cntrs->setSelectionMode(QAbstractItemView::SingleSelection);
  m_view_cntrs->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_view_cntrs->setModel(m_model_cntrs);
  m_view_cntrs->setItemDelegate(new cntrs_item_delegate_c(data->vec(), this));

  vbox2->addWidget(m_view_cntrs);
  {
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setSpacing(6);
    hbox->addStretch();

    QPushButton *add = new QPushButton(QIcon(QPixmap(":/icons/add3.png")), QString());
    add->setToolTip(IDSTR_ADD_LAYER_S);
    add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hbox->addWidget(add);
    hbox->setAlignment(add, Qt::AlignRight);
    connect(add, SIGNAL(clicked()), this, SLOT(add_cntrs_clicked()));

    QPushButton *rem = new QPushButton(QIcon(QPixmap(":/icons/buttonminus.png")), QString());
    rem->setToolTip(IDSTR_REMOVE_LAYER);
    rem->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hbox->addWidget(rem);
    hbox->setAlignment(rem, Qt::AlignRight);
    connect(rem, SIGNAL(clicked()), this, SLOT(rem_cntrs_clicked()));

    vbox2->addLayout(hbox);
  }

  setWindowTitle(IDSTR_STARTUP_FILES);

  QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
  vbox0->addWidget(bbox);

  connect(bbox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(accept()));
  connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

  int vcntrs_w = 0;
  for (int i = 0; i < m_view_cntrs->model()->columnCount(); i++)
    vcntrs_w += m_view_cntrs->columnWidth(i);
  vcntrs_w += m_view_cntrs->contentsMargins().left() + m_view_cntrs->contentsMargins().right();
  m_view_cntrs->setMinimumWidth(vcntrs_w);

  int vcntrs_h = 0;
  for (int i = 0; i < m_view_cntrs->model()->rowCount(); i++)
    vcntrs_h += m_view_cntrs->rowHeight(i);
  vcntrs_h += m_view_cntrs->horizontalHeader()->sizeHint().height();

  QList<int> sz = { 300 + btn_h, vcntrs_h + btn_h };
  splitter->setSizes(sz);

  resize(vcntrs_w, vcntrs_h + 300 + 200);
}

void dlg_startup_c::accept()
{
  info_data_c *data = m_win->m_data;
  data_tmpl_t tmpl;
  data->data_tmpl(tmpl);

  int f = m_win->m_sets->save_ini(SETS_GROUP_GRIB_STARTUP | SETS_GROUP_CNTRS_STARTUP | SETS_GROUP_DATE_TIME, &tmpl);
  m_win->set_msg(f ? IDSTR_SAVE_OP_SUCCEEDED : IDSTR_OP_FAILED, false, true);

  m_win->m_dlg_startup = nullptr;

  QDialog::accept();
}

void dlg_startup_c::reject()
{
  m_win->m_dlg_startup = nullptr;

  QDialog::reject();
}

QList<QStandardItem*> dlg_startup_c::cntrs_add_row(info_cntrs_c *info)
{
  QList<QStandardItem*> tmp;
  QStandardItem *item = new QStandardItem(info->name());
  item->setEditable(false);
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem(info->dbf_plparam());
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem();
  item->setBackground(QBrush(info->text_c()));
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem();
  item->setBackground(QBrush(info->line_c()));
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem(QString::number(info->line_w()));
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem(dash_line_names[info->dash()]);
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem(info->font_face());
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);
  item = new QStandardItem(QString::number(info->font_size()));
  item->setData(QVariant::fromValue<info_cntrs_c*>(info));
  tmp.append(item);

  return tmp;
}

QWidget* cntrs_item_delegate_c::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  int col = index.column();
  if (!index.isValid() || !index.data(Qt::UserRole + 1).isValid())
    return nullptr;
  info_cntrs_c *info = index.data(Qt::UserRole + 1).value<info_cntrs_c*>();
  if (!info) {
    my_log("err #%s\n", "0xde00e8fa");
    return nullptr;
  }

  QWidget *editor = nullptr;
  switch (col) {
  case CNTRS_FILE_NAME:
    editor = (QWidget *) new QLineEdit(parent);
    break;
  case CNTRS_TEXT_COLOR: {
    editor = (QWidget *) new QPushButton(parent);
    QPushButton *btn = (QPushButton *)editor;
    QColor color = QColorDialog::getColor(info->text_c());
    if (color.isValid()) {
      QPalette palette = btn->palette();
      palette.setColor(QPalette::Button, color);
      btn->setPalette(palette);

      info->set_text_c(color);
    }
    else {
      QPalette palette = btn->palette();
      palette.setColor(QPalette::Button, info->text_c());
      btn->setPalette(palette);
    }
    connect(btn, SIGNAL(clicked()), this, SLOT(commit_and_close()));
    break;
  }
  case CNTRS_LINE_COLOR: {
    editor = (QWidget *) new QPushButton(parent);
    QPushButton *btn = (QPushButton *)editor;
    QColor color = QColorDialog::getColor(info->line_c());
    if (color.isValid()) {
      QPalette palette = btn->palette();
      palette.setColor(QPalette::Button, color);
      btn->setPalette(palette);

      info->set_line_c(color);
    }
    else {
      QPalette palette = btn->palette();
      palette.setColor(QPalette::Button, info->line_c());
      btn->setPalette(palette);
    }
    connect(btn, SIGNAL(clicked()), this, SLOT(commit_and_close()));
    break;
  }
  case CNTRS_LINE_WIDTH: {
    editor = (QWidget *) new QDoubleSpinBox(parent);
    QDoubleSpinBox *dsb = (QDoubleSpinBox *)editor;
    dsb->setSingleStep(0.1);
    dsb->setMinimum(0.1);
    dsb->setMaximum(10);
    break;
  }
  case CNTRS_DASHED: {
    editor = (QWidget *) new QComboBox(parent);
    QComboBox *cb = (QComboBox *)editor;
    QStringList sl;
    for (int i = 0; i < DASH_LINE_NAME_NUM; i++)
      sl.append(dash_line_names[i]);
    cb->addItems(sl);
    break;
  }
  case CNTRS_FONT:
    editor = (QWidget *) new QFontComboBox(parent);
    break;
  case CNTRS_FONT_SIZE: {
    editor = (QWidget *) new QSpinBox(parent);
    QSpinBox *sb = (QSpinBox *)editor;
    sb->setSingleStep(1);
    sb->setMinimum(1);
    sb->setMaximum(100);
    break;
  }
  default:
    editor = QStyledItemDelegate::createEditor(parent, option, index);
  }
  return editor;
}

void cntrs_item_delegate_c::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  int col = index.column();
  if (!index.isValid() || !index.data(Qt::UserRole + 1).isValid())
    return;
  info_cntrs_c *info = index.data(Qt::UserRole + 1).value<info_cntrs_c*>();
  if (!info) {
    my_log("err #%s\n", "0xb0531712");
    return;
  }

  switch (col) {
  case CNTRS_FILE_NAME: {
    QLineEdit *le = qobject_cast<QLineEdit *>(editor);
    if (le)
      le->setText(info->name());
    break;
  }
  case CNTRS_LINE_WIDTH: {
    QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox *>(editor);
    if (dsb)
      dsb->setValue(info->line_w());
    break;
  }
  case CNTRS_DASHED: {
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    if (cb)
      cb->setCurrentIndex(info->dash());
    break;
  }
  case CNTRS_FONT: {
    QFontComboBox *fcb = qobject_cast<QFontComboBox *>(editor);
    if (fcb) {
      QFont font(info->font_face(), info->font_size());
      fcb->setCurrentFont(font);
    }
    break;
  }
  case CNTRS_FONT_SIZE: {
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    if (sb)
      sb->setValue(info->font_size());
    break;
  }
  default:
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void cntrs_item_delegate_c::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  int col = index.column();
  if (!index.isValid() || !index.data(Qt::UserRole + 1).isValid())
    return;
  info_cntrs_c *info = index.data(Qt::UserRole + 1).value<info_cntrs_c*>();
  if (!info) {
    my_log("err #%s\n", "0x3469b19f");
    return;
  }

  switch (col) {
  case CNTRS_DBF_PLS_PARAM: {
    QLineEdit *le = qobject_cast<QLineEdit *>(editor);
    if (le) {
      info->set_dbf_plparam(le->text());
      model->setData(index, info->dbf_plparam());
    }
    break;
  }
  case CNTRS_TEXT_COLOR: {
    QPushButton *btn = qobject_cast<QPushButton *>(editor);
    info->set_text_c(btn->palette().color(QPalette::Button));
    model->setData(index, info->text_c());

    QStandardItem *item = qobject_cast<QStandardItemModel *>(model)->itemFromIndex(index);
    if (item) {
      item->setBackground(QBrush(info->text_c()));
      info->cntrs_ptr()->set_text_color(info->text_c());
    }
    break;
  }
  case CNTRS_LINE_COLOR: {
    QPushButton *btn = (QPushButton *)editor;
    info->set_line_c(btn->palette().color(QPalette::Button));
    model->setData(index, info->line_c());

    QStandardItem *item = qobject_cast<QStandardItemModel *>(model)->itemFromIndex(index);
    if (item) {
      item->setBackground(QBrush(info->line_c()));
      info->cntrs_ptr()->set_line(info->line_c(), info->line_w(), info->dash());
    }
    break;
  }
  case CNTRS_LINE_WIDTH: {
    QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox *>(editor);
    if (dsb) {
      info->set_line_w((float)dsb->value());
      model->setData(index, QString::number(info->line_w()));
      info->cntrs_ptr()->set_line(info->line_c(), info->line_w(), info->dash());
    }
    break;
  }
  case CNTRS_DASHED: {
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    if (cb) {
      info->set_dash(cb->currentIndex());
      model->setData(index, dash_line_names[info->dash()]);
      info->cntrs_ptr()->set_line(info->line_c(), info->line_w(), info->dash());
    }
    break;
  }
  case CNTRS_FONT: {
    QFontComboBox *fcb = qobject_cast<QFontComboBox *>(editor);
    if (fcb) {
      info->set_font_face(fcb->currentFont().family());
      model->setData(index, info->font_face());
      info->cntrs_ptr()->set_font(info->font_face(), info->font_size());
    }
    break;
  }
  case CNTRS_FONT_SIZE: {
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    if (sb) {
      info->set_font_size(sb->value());
      model->setData(index, QString::number(info->font_size()));
      info->cntrs_ptr()->set_font(info->font_face(), info->font_size());
    }
    break;
  }
  }
}

void cntrs_item_delegate_c::commit_and_close()
{
  if (QWidget *pb = qobject_cast<QWidget *>(sender())) {
    emit commitData(pb);
    emit closeEditor(pb);
  }
}

void dlg_startup_c::add_grib_clicked()
{
  info_data_c *data = m_win->m_data;

  QString wd = m_win->m_persfold + DIR_SEPARATOR_S MY_NWP_IMPORT;
  QStringList fnames = QFileDialog::getOpenFileNames(this, IDSTR_OPEN, wd
#ifdef _WINDOWS
    , "Grib2 (*.f???;*.grib2;*.grb2)"
#endif
  );
  if (fnames.isEmpty() || fnames[0].isEmpty()) {
    m_win->set_msg(IDSTR_SELECT_DATA_PLANES, false, true);
    return;
  }
  m_win->wait_cursor(true);

  int n = fnames[0].lastIndexOf(DIR_SEPARATOR_S, -1);
  n = fnames[0].lastIndexOf(DIR_SEPARATOR_S, n - 1);
  n++;

  set<string> id_set;
  for (int i = 0; i < fnames.size(); i++) {
    datefc_c datefc;
    QString name = fnames[i].mid(n, -1);
    string role;
    QByteArray b = name.toLatin1();
    if (!datefc.parse_img_name(b.constData(), role)) {
      my_log("err #%s\n", "0x628359cf");
      goto end;
    }
    string id = datefc.runfc_str() + role;
    if (id_set.find(id) != id_set.end())
      continue;
    id_set.insert(id);

    vector<grib_c*> grbs;
    if (role == GFS_025) {
      bool inplace;
      grib_c *grib = data->add_slot(datefc, &inplace);
      rtlv_rf_c par;
      if (!grib->read_grib(fnames[i], data->files_ptr(), role, par)) {
        my_log("err #%s\n", "0xce30fdd8");
        delete grib;
        continue;
      }
      data->set_cur_grib(par, grib);
      if (!inplace)
        grbs.push_back(grib);
    }
  }
  if (data->gdata_empty())
    goto end;
  {
    bool clear;
    if (!data->cur_gdata_ptr()->make_grib(clear)) {
      my_log("err #%s\n", "0x4833c725");
      goto end;
    }
    m_tag_tree.tree_from_gribs(m_win->m_data->gribs());
    m_tag_tree.expand_all(0);

    m_win->m_tag_tree.tree_from_gribs(m_win->m_data->gribs());
    m_win->set_cur(true, false);

    m_win->grib_done(clear);
  }
end:
  if (m_win->m_data->gdata_empty())
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
  m_win->wait_cursor(false);
}

void dlg_startup_c::add_cntrs_clicked()
{
  QString wd = m_win->m_persfold + DIR_SEPARATOR_S MY_SHP_IMPORT;
  QStringList fnames = QFileDialog::getOpenFileNames(this, IDSTR_OPEN, wd, "Shp (*.shp)");
  if (fnames.isEmpty() || fnames[0].isEmpty())
    return;
  m_win->wait_cursor(true);

  int n;
  n = fnames[0].lastIndexOf(DIR_SEPARATOR_S, -1);
  n++;
  QStandardItem *root = m_model_cntrs->invisibleRootItem();

  for (int i = 0; i < fnames.size(); i++) {
    QFileInfo finfo(fnames[i]);
    if (!finfo.exists()) {
      my_log("err #%s\n", "0x71cd3f97");
      m_win->set_msg(IDSTR_OP_FAILED, true, false);
      continue;
    }
    info_cntrs_c *info = new info_cntrs_c(m_win);
    info->set_name(fnames[i].mid(n, -1));
    if (!info->make_cntrs()) {
      m_win->set_msg(IDSTR_OP_FAILED, true, false);
      continue;
    }
    root->appendRow(cntrs_add_row(info));
  }
  m_win->wait_cursor(false);
}

void dlg_startup_c::rem_cntrs_clicked()
{
  QModelIndexList list = m_view_cntrs->selectionModel()->selectedIndexes();
  if (!list.size())
    return;
  QModelIndex index = list.at(0);
  if (index.isValid() && index.data(Qt::UserRole + 1).isValid()) {
    info_cntrs_c *info = index.data(Qt::UserRole + 1).value<info_cntrs_c*>();
    if (info) {
      info_data_c *data = m_win->m_data;
      data->vec_ptr()->erase(std::find(data->vec_ptr()->begin(), data->vec_ptr()->end(), info));
      delete info;
      if (!data->vec()->size()) {
        delete data->vec_ptr();
        data->set_vec(nullptr);
      }
      m_model_cntrs->removeRow(index.row());
      return;
    }
  }
  m_win->set_msg(IDSTR_SELECT_FILE_NAME_COLUMN, true, false);
}

void dlg_startup_c::rem_all_clicked()
{
  m_win->close_all_clicked();
  m_tag_tree.tree_done();
}

