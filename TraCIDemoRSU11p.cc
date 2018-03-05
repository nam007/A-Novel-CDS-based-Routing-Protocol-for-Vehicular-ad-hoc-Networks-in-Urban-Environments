

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"



Define_Module(TraCIDemoRSU11p);

void TraCIDemoRSU11p::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void TraCIDemoRSU11p::onWSM(WaveShortMessage* wsm) {

    if(myId == wsm->getRecipientAddress() && myId == 12){
        cout<<"got it at rsu "<<12<<endl;
        WaveShortMessage* wsm = new WaveShortMessage();
                             populateWSM(wsm);
                          //   wsm->setRecipientAddress(-1);
                             wsm->setSenderAddress(12);
                             sendDown(wsm);
    }


    //this rsu repeats the received traffic update in 2 seconds plus some random delay
        if(myId ==wsm->getRecipientAddress() && wsm->getFl()==false){
            wsm->setFl(true);

            string wrid=wsm->getWrid();
            simtime_t t = simTime();
            t = t - wsm->getCreationTime();
      //      cout<<"rsu creation "<<wsm->getCreationTime()<<" "<<t<<endl;
       //  cout<<"my id "<<myId<<" wsm info is "<<wsm->getWsmData()<<"sender "<<wsm->getSenderAddress()<<" road "<<wsm->getWrid()<<" recipeint id "<<wsm->getRecipientAddress()<<endl;

         WaveShortMessage* wsm = new WaveShortMessage();
                     populateWSM(wsm);
                     wsm->setRecipientAddress(-1);
                     //wsm->setName("RAP");
                     wsm->setSenderAddress(myId);
                     //wsm->setWsmData("RAP");
                     wsm->setWrid(wrid.c_str());

                     wsm->setFl(true);
                     wsm->setDelay(2*t);

                     sendDown(wsm->dup());



        }
       // sendDelayedDown(wsm->dup(), 2 + u niform(0 .01,0.2));
}


void TraCIDemoRSU11p::onBSM(BasicSafetyMessage* bsm) {
    //cout<<" my id is rsu "<<myId<<endl;

}

