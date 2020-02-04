#include "BodySuperimposerItem.h"
#include "BodyItem.h"
#include <cnoid/ItemManager>
#include <cnoid/PutPropertyFunction>
#include <cnoid/SceneBody>
#include <cnoid/CloneMap>
#include <cnoid/Archive>
#include "gettext.h"

using namespace std;
using namespace cnoid;

namespace cnoid {

class BodySuperimposerItem::Impl
{
public:
    BodySuperimposerItem* self;
    BodyItem* bodyItem;
    ScopedConnection bodyItemConnection;
    BodyPtr superimposedBody;
    SgSwitchableGroupPtr topSwitch;
    SgUnpickableGroupPtr mainGroup;
    SceneBodyPtr sceneBody;
    float transparency;
    SgUpdate sgUpdate;
    CloneMap cloneMap;

    Impl(BodySuperimposerItem* self);
    Impl(BodySuperimposerItem* self, const Impl& org);
    void setBodyItem(BodyItem* newBodyItem);
    void setTransparency(float t);
};

}


void BodySuperimposerItem::initializeClass(ExtensionManager* ext)
{
    ext->itemManager().registerClass<BodySuperimposerItem>(N_("BodySuperimposerItem"));
}


BodySuperimposerItem::BodySuperimposerItem()
{
    impl = new Impl(this);
}


BodySuperimposerItem::Impl::Impl(BodySuperimposerItem* self)
{
    bodyItem = nullptr;
    topSwitch = new SgSwitchableGroup;
    topSwitch->setTurnedOn(false);
    mainGroup = new SgUnpickableGroup;
    topSwitch->addChild(mainGroup);
    transparency = 0.7;
    SgObject::setNonNodeCloning(cloneMap, false);
}


BodySuperimposerItem::BodySuperimposerItem(const BodySuperimposerItem& org)
    : Item(org)
{
    impl = new Impl(this, *org.impl);
    setName(org.name());
}


BodySuperimposerItem::Impl::Impl(BodySuperimposerItem* self, const Impl& org)
    : Impl(self)
{
    transparency = org.transparency;
}

    

BodySuperimposerItem::~BodySuperimposerItem()
{
    delete impl;
}


Item* BodySuperimposerItem::doDuplicate() const
{
    return new BodySuperimposerItem(*this);
}


void BodySuperimposerItem::setName(const std::string& name)
{
    Item::setName(name);
    impl->topSwitch->setName(name);
}


void BodySuperimposerItem::onPositionChanged()
{
    impl->setBodyItem(findOwnerItem<BodyItem>());
}


void BodySuperimposerItem::Impl::setBodyItem(BodyItem* newBodyItem)
{
    if(newBodyItem != bodyItem){
        bodyItem = newBodyItem;
        superimposedBody.reset();
        mainGroup->clearChildren();
        sceneBody.reset();

        if(bodyItem){
            superimposedBody = bodyItem->body()->clone(cloneMap);
            sceneBody = new SceneBody(superimposedBody);
            sceneBody->makeTransparent(transparency, cloneMap);
            mainGroup->addChild(sceneBody);
            cloneMap.clear();

            bodyItemConnection =
                bodyItem->sigKinematicStateChanged().connect(
                    [&](){ self->clearSuperimposition(); });
        }
    }
}


Body* BodySuperimposerItem::superimposedBody()
{
    return impl->superimposedBody;
}


const Body* BodySuperimposerItem::superimposedBody() const
{
    return impl->superimposedBody;
}


SgNode* BodySuperimposerItem::getScene()
{
    return impl->topSwitch;
}
    

void BodySuperimposerItem::setTransparency(float transparency)
{
    impl->setTransparency(transparency);
}


void BodySuperimposerItem::Impl::setTransparency(float t)
{
    if(t != transparency){
        if(sceneBody){
            sceneBody->makeTransparent(t, cloneMap);
            cloneMap.clear();
        }
        transparency = t;
    }
}


void BodySuperimposerItem::updateSuperimposition()
{
    if(impl->sceneBody){
        impl->sceneBody->updateLinkPositions(impl->sgUpdate);
        impl->topSwitch->setTurnedOn(true, true);
    }
}


void BodySuperimposerItem::clearSuperimposition()
{
    impl->topSwitch->setTurnedOn(false, true);
}


void BodySuperimposerItem::doPutProperties(PutPropertyFunction& putProperty)
{
    putProperty.min(0.0).max(1.0);
    putProperty(_("Transparency"), static_cast<double>(impl->transparency),
                [&](double value){ impl->setTransparency(value); return true; });
}


bool BodySuperimposerItem::store(Archive& archive)
{
    archive.write("transparency", impl->transparency);
    return true;
}


bool BodySuperimposerItem::restore(const Archive& archive)
{
    impl->setTransparency(archive.get("transparency", impl->transparency));
    return true;
}