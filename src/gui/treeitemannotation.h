/*
 * PDFedit - free program for PDF document manipulation.
 * Copyright (C) 2006, 2007  PDFedit team:      Michal Hocko,
 *                                              Miroslav Jahoda,
 *                                              Jozef Misutka,
 *                                              Martin Petricek
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit
 */
#ifndef __TREEITEMANNOTATION_H__
#define __TREEITEMANNOTATION_H__

#include "treeitemabstract.h"
#include <boost/shared_ptr.hpp>
#include <vector>
class QString;
namespace pdfobjects {
 class CPage;
 class CAnnotation;
}

namespace gui {

using namespace pdfobjects;

class TreeData;

/**
 Class holding one Annotation in tree.
 \brief Tree item containing CAnnotation
 */
class TreeItemAnnotation : public TreeItemAbstract {
public:
 TreeItemAnnotation(TreeData *_data,Q_ListView *parent,boost::shared_ptr<CAnnotation> pdfObj,boost::shared_ptr<CPage> _page,const QString name=QString::null,Q_ListViewItem *after=NULL,const QString &nameId=NULL);
 TreeItemAnnotation(TreeData *_data,Q_ListViewItem *parent,boost::shared_ptr<CAnnotation> pdfObj,boost::shared_ptr<CPage> _page,const QString name=QString::null,Q_ListViewItem *after=NULL,const QString &nameId=NULL);
 virtual ~TreeItemAnnotation();
 boost::shared_ptr<CAnnotation> getObject();
 //From TreeItemAbstract interface
 virtual bool haveChild();
 virtual TreeItemAbstract* createChild(const QString &name,ChildType typ,Q_ListViewItem *after=NULL);
 virtual void reloadSelf();
 virtual ChildType getChildType(const QString &name);
 virtual QStringList getChildNames();
 virtual QSCObject* getQSObject();
 virtual void remove();
 virtual bool validChild(const QString &name,Q_ListViewItem *oldChild);
private:
 void init();
private:
 /** Annotation stored inside */
 boost::shared_ptr<CAnnotation> obj;
 /** CPage containing this annotation */
 boost::shared_ptr<CPage> page;
 /** Annotation title (name) */
 QString title;
};

} // namespace gui

#endif
