#include "armature.h"

armature::armature(ofVec3f cntr, float w, float h, float d, float bs){
    center = cntr;
    width  = w;
    height = h;
    depth = d;
    blockSide = bs;//ideally for a standar rubicks cube, is 3 :: height/3 or width/3
    side = height/blockSide;
    
    btogglePlanes = false;
    
    degX=0;
    degY=0;
    degZ=0;
}
//--------------------------------------------------------------
void armature::setup(){
    plane1.set( width, height );
    plane1.setPosition(0-(width/2)+side+1, 0, center.z);
    plane1.rotate(90, 0, 1.0, 0.0);
    
    plane2.set( width, height );
    plane2.setPosition(0+(width/2)-side-1, 0, center.z);
    plane2.rotate(90, 0, 1.0, 0.0);
    
    
    plane3.set( width, height);
    plane3.setPosition(0, 0+(height/2)-side-1, center.z);
    plane3.rotate(90, 1.0, 0.0, 0.0);
    plane3.rotate(90, 0.0, 1.0, 0.0);
    
    plane4.set( width, height);
    plane4.setPosition(0, 0-(height/2)+side+1, center.z);
    plane4.rotate(90, 1.0, 0.0, 0.0);
    plane4.rotate(90, 0.0, 1.0, 0.0);
    
    
    plane5.set( width, height );
    plane5.setPosition(0, 0, center.z+(width/2)-side-1);
    plane5.rotate(0, 0, 1.0, 0.0);
    
    plane6.set( width, height );
    plane6.setPosition(0, 0, center.z-(width/2)+side+1);
    plane6.rotate(0, 0, 1.0, 0.0);
    ///////// CYLINDERS //////////////////////////////////////////////////////////////////////
    cylinder1.set(height/30,height);
    cylinder1.setPosition(0, 0, center.z);
    cylinder1.setResolution(20, 1, 4);
    
    cylinder2.set(height/30,height);
    cylinder2.setPosition(0, 0, center.z);
    cylinder2.setResolution(20, 1, 4);
    cylinder2.rotate(90, 1.0, 0.0, 0.0);
    
    cylinder3.set(height/30,height);
    cylinder3.setPosition(0, 0, center.z);
    cylinder3.setResolution(20, 1, 4);
    cylinder3.rotate(90, 0.0, 0.0, 1.0);
    //END///// CYLINDERS //////////////////////////////////////////////////////////////////////
    
    /////////BOXES//////////////////////////////////////////////////////////////////////
    //r//
    box1.set( side,side,depth);//width, height, thick
    box1.setPosition(0-(width/2), 0, center.z);
    box1.rotate(90, 0, 1.0, 0.0);
    
    box2.set( side,side,depth);
    box2.setPosition(0+width/2, 0, center.z);
    box2.rotate(90, 0, 1.0, 0.0);
    //g//
    box3.set( side,side,depth);
    box3.setPosition(0, 0-(height/2), center.z);
    box3.rotate(90, 1.0, 0.0, 0.0);
    box3.rotate(90, 0.0, 1.0, 0.0);
    
    box4.set( side,side,depth);
    box4.setPosition(0, 0+height/2, center.z);
    box4.rotate(90, 1.0, 0.0, 0.0);
    box4.rotate(90, 0.0, 1.0, 0.0);
    //b//
    box5.set( side,side,depth);
    box5.setPosition(0, 0, center.z+width/2);
    box5.rotate(0, 0, 1.0, 0.0);
    
    box6.set( side,side,depth);
    box6.setPosition(0, 0, center.z-width/2);
    box6.rotate(0, 0, 1.0, 0.0);
    //END/////BOXES//////////////////////////////////////////////////////////////////////
    
//    sphere.setPosition(0, 0, center.z);
//    sphere.setRadius( 10 );
//    ofSetSphereResolution(24);

}

//--------------------------------------------------------------
void armature::update(){
    degX +=0.1;
    degY +=0;
    degZ +=0;
}

void armature::togglePlanes(){
    if(btogglePlanes == true){
        cout << "Time: " << ofGetElapsedTimef() << endl;
        btogglePlanes =false;
    }else{
        btogglePlanes=true;
    }
}

//--------------------------------------------------------------
void armature::draw(){
    ofPushMatrix();
    ofTranslate(center.x,center.y);
    ofRotate(degX, 1.0, 0.0, 0.0);
    ofRotate(degY, 0, 1, 0);
    ofRotate(degZ, 0, 0, 1);
    
    
    ////////////////////boxes//////////////////////////////////////////////////////////////////////
    // Box1 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(255,0,0);
//        if(mode == 3) {
//            box1.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box1.restoreTransformGL();
//        } else {
            box1.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box1.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box1.drawWireframe();
//        box1.setScale(1.f);
//    }
    // Box2 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(255,0,0);
//        if(mode == 3) {
//            box2.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box2.restoreTransformGL();
//        } else {
            box2.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(250, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box2.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box2.drawWireframe();
//        box2.setScale(1.f);
//    }
    // Box3 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(0,255,0);
//        if(mode == 3) {
//            box2.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box3.restoreTransformGL();
//        } else {
            box3.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box3.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box3.drawWireframe();
//        box3.setScale(1.f);
//    }
    // Box4 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(0,255,0);
//        if(mode == 3) {
//            box4.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box4.restoreTransformGL();
//        } else {
            box4.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box4.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box4.drawWireframe();
//        box4.setScale(1.f);
//    }
    // Box5 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(0,0,255);
//        if(mode == 3) {
//            box5.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box5.restoreTransformGL();
//        } else {
            box5.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box5.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box5.drawWireframe();
//        box5.setScale(1.f);
//    }
    // Box6 ////---------------------------------------------------------
//    if(bFill) {
        ofFill();
        ofSetColor(0,0,255);
//        if(mode == 3) {
//            box6.transformGL();
//            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
//                ofPushMatrix();
//                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
//                boxSides[i].draw();
//                ofPopMatrix();
//            }
//            box6.restoreTransformGL();
//        } else {
            box6.draw();
//        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        box6.setScale(1.01f);
//        if(mode == 3) {
//            ofSetColor(255);
//        }
//        box6.drawWireframe();
//        box6.setScale(1.f);
//    }
    
    ////////////////// Sphere ////////////////////////////////////////////////////////////////////
//    if(mode == 3) {
//        sphere.setMode( OF_PRIMITIVE_TRIANGLES );
//        triangles = sphere.getMesh().getUniqueFaces();
//    }
    
//    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            float angle = ofGetElapsedTimef()*3.2;
            float strength = (sin( angle+.25 )) * .5f * 5.f;
            ofVec3f faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                // store the face normal here.
                // we change the vertices, which makes the face normal change
                // every time that we call getFaceNormal //
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength);
                }
            }
//            sphere.getMesh().setFromTriangles( triangles );
        }
//        sphere.draw();
    
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill) ofSetColor(255);
//        sphere.setScale(1.01f);
//        sphere.drawWireframe();
//        sphere.setScale(1.f);
//    }
    
    // Planes ////////////////////////////////////////////////////////////////
    if(btogglePlanes == true) {
        //        deformPlane = plane1.getMesh();
        //        // x = columns, y = rows //
        //        ofVec3f planeDims = plane1.getResolution();
        //        float planeAngleX = ofGetElapsedTimef()*3.6;
        //        float planeAngleInc = 3.f/(float)planeDims.x;
        //        ofVec3f vert;
        //        for(int i = 0; i < deformPlane.getNumIndices(); i++ ) {
        //            planeAngleX += planeAngleInc;
        //            int ii = deformPlane.getIndex( i );
        //            vert = deformPlane.getVertex( ii );
        //            vert.z += cos(planeAngleX) * 50;
        //            deformPlane.setVertex( ii, vert );
        //        }
        //    }
        
        //    if(bFill) {
        ofFill();
        
        //        if(mode == 3) {
        //            plane.transformGL();
        //            deformPlane.draw();
        //            plane.restoreTransformGL();
        //        } else {
        ofSetColor(255,250,10,150);
        plane1.draw();
        ofSetColor(255,250,10,150);
        plane2.draw();
        
        ofSetColor (255,0,0,150);
        plane3.draw();
        ofSetColor(255,0,0,150);
        plane4.draw();
        
        ofSetColor(0,0,255,150);
        plane5.draw();
        ofSetColor(0,0,255,150);
        plane6.draw();
        //        }
    }
    //    if(bWireframe) {
    //        ofNoFill();
    //        ofSetColor(0, 0, 0);
    //        if(!bFill) ofSetColor(255);
    //        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z+1);
    //        //if(bFill) {
    //        if( mode == 3 ) {
    //            ofSetColor(255);
    //        }
    //        plane.drawWireframe();
    //        //}
    //        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z-2);
    //        
    //    }

    
    
    ////////////////// Cylinders ////////////////////////////////////////////////////////////////////
    //cylinder 1//----------------------------------------------------------------------------
    if(mode == 3) {
        topCap      = cylinder1.getTopCapMesh();
        bottomCap   = cylinder1.getBottomCapMesh();
        body        = cylinder1.getCylinderMesh();
    }
    
//    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            cylinder1.transformGL();
            ofPushMatrix(); {
                if(topCap.getNumNormals() > 0) {
                    ofTranslate( topCap.getNormal(0) * (cos(ofGetElapsedTimef()*5)+1)*.5f * 100 );
                    topCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                if(bottomCap.getNumNormals() > 0) {
                    ofTranslate( bottomCap.getNormal(0) * (cos(ofGetElapsedTimef()*4)+1)*.5f * 100 );
                    bottomCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef()*3)+1)*.5f + .2;
                ofScale( scale, scale, scale );
                body.draw();
            } ofPopMatrix();
            cylinder1.restoreTransformGL();
        } else {
            cylinder1.draw();
        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill || mode == 3) ofSetColor(255);
//        cylinder1.setScale(1.01f);
//        cylinder1.drawWireframe();
//        cylinder1.setScale(1.0f);
//    }
    //cylinder 2//----------------------------------------------------------------------------
    if(mode == 3) {
        topCap      = cylinder2.getTopCapMesh();
        bottomCap   = cylinder2.getBottomCapMesh();
        body        = cylinder2.getCylinderMesh();
    }
    
//    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            cylinder2.transformGL();
            ofPushMatrix(); {
                if(topCap.getNumNormals() > 0) {
                    ofTranslate( topCap.getNormal(0) * (cos(ofGetElapsedTimef()*5)+1)*.5f * 100 );
                    topCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                if(bottomCap.getNumNormals() > 0) {
                    ofTranslate( bottomCap.getNormal(0) * (cos(ofGetElapsedTimef()*4)+1)*.5f * 100 );
                    bottomCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef()*3)+1)*.5f + .2;
                ofScale( scale, scale, scale );
                body.draw();
            } ofPopMatrix();
            cylinder2.restoreTransformGL();
        } else {
            cylinder2.draw();
        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill || mode == 3) ofSetColor(255);
//        cylinder2.setScale(1.01f);
//        cylinder2.drawWireframe();
//        cylinder2.setScale(1.0f);
//    }
    //cylinder 3//----------------------------------------------------------------------------
    if(mode == 3) {
        topCap      = cylinder3.getTopCapMesh();
        bottomCap   = cylinder3.getBottomCapMesh();
        body        = cylinder3.getCylinderMesh();
    }
    
//    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            cylinder3.transformGL();
            ofPushMatrix(); {
                if(topCap.getNumNormals() > 0) {
                    ofTranslate( topCap.getNormal(0) * (cos(ofGetElapsedTimef()*5)+1)*.5f * 100 );
                    topCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                if(bottomCap.getNumNormals() > 0) {
                    ofTranslate( bottomCap.getNormal(0) * (cos(ofGetElapsedTimef()*4)+1)*.5f * 100 );
                    bottomCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef()*3)+1)*.5f + .2;
                ofScale( scale, scale, scale );
                body.draw();
            } ofPopMatrix();
            cylinder3.restoreTransformGL();
        } else {
            cylinder3.draw();
        }
//    }
//    if(bWireframe) {
//        ofNoFill();
//        ofSetColor(0, 0, 0);
//        if(!bFill || mode == 3) ofSetColor(255);
//        cylinder3.setScale(1.01f);
//        cylinder3.drawWireframe();
//        cylinder3.setScale(1.0f);
//    }
    
    ///////////////////////////////////////////////////////////////////////////////////////

    ofPopMatrix();
}
