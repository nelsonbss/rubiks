#include "GuiImage.h"

GuiImage::GuiImage(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
	size.x = img.getWidth() * scale;
	size.y = img.getHeight() * scale;
    haveArabic = false;
    map<string,string>::iterator mIter;
    mIter = attrs.find("arabic");
    /*
	if(mIter != attrs.end()){
        haveArabic = true;
        imgArabic.loadImage(attrs["arabic"]);
    }
	*/
}

void GuiImage::execute(){
}

void GuiImage::nodeDraw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	//if(haveArabic && SceneManager::Instance()->getDisplayArabic()){
    //    imgArabic.draw(pos.x,pos.y);
	//} else {
    img.draw(drawPos.x,drawPos.y, size.x, size.y);
    //}
}

void GuiImage::update(string _subName, Subject *_sub){
}
