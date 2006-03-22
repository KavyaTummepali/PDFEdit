/** @file
StringProperty - class for widget containing
 one editable property of type "String"
*/

#include "stringproperty.h"
#include <stdlib.h>

/** Default constructor of property item
 @param parent parent Property Editor containing this control
 @param name name of his property
 @param flags flags of this property items (default 0)
 */
StringProperty::StringProperty(const QString &_name, QWidget *parent/*=0*/, PropertyFlags _flags/*=0*/)
 : Property(_name,parent,_flags) {
 ed=new QLineEdit(this,"stringproperty_edit");
/* ed->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
 ed->setText(_name);//example text*/
}

/** return size hint of this property editing control */
QSize StringProperty::sizeHint() const {
 return ed->sizeHint();
}

/** Called on resizing of property editing control */
void StringProperty::resizeEvent (QResizeEvent *e) {
 ed->setFixedSize(e->size());
}

/** default destructor */
StringProperty::~StringProperty() {
 delete ed;
}

/** write internal value to given PDF object
 @param pdfObject Object to write to
 */
void StringProperty::writeValue(void *pdfObject) {
 //TODO
}
/** read internal value from given PDF object
 @param pdfObject Object to read from
 */
void StringProperty::readValue(void *pdfObject) {
 //TODO
}
