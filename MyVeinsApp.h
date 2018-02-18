//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef __VEINS_MYVEINSAPP_H_
#define __VEINS_MYVEINSAPP_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;

/**
 * @brief
 * This is a stub for a typical Veins application layer.
 * Most common functions are overloaded.
 * See MyVeinsApp.cc for hints
 *
 * @author David Eckhoff
 *
 */
struct neighbour{
                    int id;
                    int sid;
                    Coord pos;
                    Coord spd;
                    double dist;
                    double x;
                    double y;
                    string rid;
                    int b=0;
                    double SF;
                    simtime_t t;
                    double vi;
                    double di;
               //     bool ta=false;
                };
struct pre_pos{
    int id;
    double x;
    double y;
};


class MyVeinsApp : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
        virtual void finish();


  //

    protected:
        virtual void onBSM(BasicSafetyMessage* bsm);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
        vector<struct neighbour> v;
        struct pre_pos p;
        virtual double calculateQ(int id);

        virtual bool return_Value();
        virtual int Find_back(int id);
    };

#endif
