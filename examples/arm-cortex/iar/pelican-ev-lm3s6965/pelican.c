/*****************************************************************************
* Model: pelican.qm
* File:  ./pelican.c
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
#include "qpn_port.h"
#include "bsp.h"
#include "pelican.h"

/*Q_DEFINE_THIS_FILE*/

#define CARS_GREEN_MIN_TOUT (QTimeEvtCtr)(BSP_TICKS_PER_SEC * 8U)
#define CARS_YELLOW_TOUT    (QTimeEvtCtr)(BSP_TICKS_PER_SEC * 3U)
#define PEDS_WALK_TOUT      (QTimeEvtCtr)(BSP_TICKS_PER_SEC * 3U)
#define PEDS_FLASH_TOUT     (QTimeEvtCtr)(BSP_TICKS_PER_SEC / 5U)
#define PEDS_FLASH_NUM      (uint8_t)(5U * 2U)
#define OFF_FLASH_TOUT      (QTimeEvtCtr)(BSP_TICKS_PER_SEC / 2U)

/* Pelican class declaration -----------------------------------------------*/
/* @(/1/0) .................................................................*/
/** 
* PEdestrian LIght CONtrolled (PELICAN) crossing
*/
typedef struct PelicanTag {
/* protected: */
    QActive super;

/* private: */
    uint8_t flashCtr;
} Pelican;

/* protected: */
static QState Pelican_initial(Pelican * const me);
static QState Pelican_operational(Pelican * const me);
static QState Pelican_carsEnabled(Pelican * const me);
static QState Pelican_carsGreen(Pelican * const me);
static QState Pelican_carsGreenNoPed(Pelican * const me);
static QState Pelican_carsGreenInt(Pelican * const me);
static QState Pelican_carsGreenPedWait(Pelican * const me);
static QState Pelican_carsYellow(Pelican * const me);
static QState Pelican_pedsEnabled(Pelican * const me);
static QState Pelican_pedsWalk(Pelican * const me);
static QState Pelican_pedsFlash(Pelican * const me);
static QState Pelican_offline(Pelican * const me);


/* Global objects ----------------------------------------------------------*/
struct PelicanTag AO_Pelican;      /* the single instance of the Pelican AO */

/* Pelican class definition ------------------------------------------------*/
/* @(/1/2) .................................................................*/
void Pelican_ctor(void) {
    QActive_ctor(&AO_Pelican.super, Q_STATE_CAST(&Pelican_initial));
}
/* @(/1/0) .................................................................*/
/* @(/1/0/1) ...............................................................*/
/* @(/1/0/1/0) */
static QState Pelican_initial(Pelican * const me) {
    return Q_TRAN(&Pelican_operational);
}
/* @(/1/0/1/1) .............................................................*/
static QState Pelican_operational(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1) */
        case Q_ENTRY_SIG: {
            BSP_signalCars(CARS_RED);
            BSP_signalPeds(PEDS_DONT_WALK);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/0) */
        case Q_INIT_SIG: {
            status = Q_TRAN(&Pelican_carsEnabled);
            break;
        }
        /* @(/1/0/1/1/1) */
        case OFF_SIG: {
            status = Q_TRAN(&Pelican_offline);
            break;
        }
        /* @(/1/0/1/1/2) */
        case TERMINATE_SIG: {
            BSP_terminate(0);
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3) ...........................................................*/
static QState Pelican_carsEnabled(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3) */
        case Q_EXIT_SIG: {
            BSP_signalCars(CARS_RED);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/0) */
        case Q_INIT_SIG: {
            status = Q_TRAN(&Pelican_carsGreen);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_operational);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3/1) .........................................................*/
static QState Pelican_carsGreen(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3/1) */
        case Q_ENTRY_SIG: {
            BSP_signalCars(CARS_GREEN);
            QActive_arm(&me->super, CARS_GREEN_MIN_TOUT);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/1) */
        case Q_EXIT_SIG: {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/1/0) */
        case Q_INIT_SIG: {
            status = Q_TRAN(&Pelican_carsGreenNoPed);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_carsEnabled);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3/1/1) .......................................................*/
static QState Pelican_carsGreenNoPed(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3/1/1) */
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenNoPed");
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/1/1/0) */
        case PEDS_WAITING_SIG: {
            status = Q_TRAN(&Pelican_carsGreenPedWait);
            break;
        }
        /* @(/1/0/1/1/3/1/1/1) */
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Pelican_carsGreenInt);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_carsGreen);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3/1/2) .......................................................*/
static QState Pelican_carsGreenInt(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3/1/2) */
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenInt");
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/1/2/0) */
        case PEDS_WAITING_SIG: {
            status = Q_TRAN(&Pelican_carsYellow);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_carsGreen);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3/1/3) .......................................................*/
static QState Pelican_carsGreenPedWait(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3/1/3) */
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenPedWait");
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/1/3/0) */
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Pelican_carsYellow);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_carsGreen);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/3/2) .........................................................*/
static QState Pelican_carsYellow(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/3/2) */
        case Q_ENTRY_SIG: {
            BSP_showState("carsYellow");
            BSP_signalCars(CARS_YELLOW);
            QActive_arm(&me->super, CARS_YELLOW_TOUT);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/2) */
        case Q_EXIT_SIG: {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/3/2/0) */
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Pelican_pedsEnabled);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_carsEnabled);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/4) ...........................................................*/
static QState Pelican_pedsEnabled(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/4) */
        case Q_EXIT_SIG: {
            BSP_signalPeds(PEDS_DONT_WALK);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/4/0) */
        case Q_INIT_SIG: {
            status = Q_TRAN(&Pelican_pedsWalk);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_operational);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/4/1) .........................................................*/
static QState Pelican_pedsWalk(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/4/1) */
        case Q_ENTRY_SIG: {
            BSP_showState("pedsWalk");
            BSP_signalPeds(PEDS_WALK);
            QActive_arm(&me->super, PEDS_WALK_TOUT);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/4/1) */
        case Q_EXIT_SIG: {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/4/1/0) */
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Pelican_pedsFlash);
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_pedsEnabled);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/4/2) .........................................................*/
static QState Pelican_pedsFlash(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/1/4/2) */
        case Q_ENTRY_SIG: {
            BSP_showState("pedsFlash");
            QActive_arm(&me->super, PEDS_FLASH_TOUT);
            me->flashCtr = (PEDS_FLASH_NUM * 2U) + 1U;
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/4/2) */
        case Q_EXIT_SIG: {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/1/4/2/0) */
        case Q_TIMEOUT_SIG: {
            /* @(/1/0/1/1/4/2/0/0) */
            if (me->flashCtr != 0) {
                --me->flashCtr;
                QActive_arm(&me->super, PEDS_FLASH_TOUT);
                /* @(/1/0/1/1/4/2/0/0/0) */
                if ((me->flashCtr & 1U) == 0U) {
                    BSP_signalPeds(PEDS_DONT_WALK);
                    status = Q_HANDLED();
                }
                /* @(/1/0/1/1/4/2/0/0/1) */
                else {
                    BSP_signalPeds(PEDS_BLANK);
                    status = Q_HANDLED();
                }
            }
            /* @(/1/0/1/1/4/2/0/1) */
            else {
                status = Q_TRAN(&Pelican_carsEnabled);
            }
            break;
        }
        default: {
            status = Q_SUPER(&Pelican_pedsEnabled);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/2) .............................................................*/
static QState Pelican_offline(Pelican * const me) {
    QState status;
    switch (Q_SIG(me)) {
        /* @(/1/0/1/2) */
        case Q_ENTRY_SIG: {
            BSP_showState("offline");
            QActive_arm(&me->super, OFF_FLASH_TOUT);
            me->flashCtr = 0U;
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/2) */
        case Q_EXIT_SIG: {
            QActive_disarm(&me->super);
            status = Q_HANDLED();
            break;
        }
        /* @(/1/0/1/2/0) */
        case Q_TIMEOUT_SIG: {
            QActive_arm(&me->super, OFF_FLASH_TOUT);
            me->flashCtr ^= 1U;
            /* @(/1/0/1/2/0/0) */
            if ((me->flashCtr & 1U) == 0U) {
                BSP_signalCars(CARS_RED);
                BSP_signalPeds(PEDS_DONT_WALK);
                status = Q_HANDLED();
            }
            /* @(/1/0/1/2/0/1) */
            else {
                BSP_signalCars(CARS_BLANK);
                BSP_signalPeds(PEDS_BLANK);
                status = Q_HANDLED();
            }
            break;
        }
        /* @(/1/0/1/2/1) */
        case ON_SIG: {
            status = Q_TRAN(&Pelican_operational);
            break;
        }
        /* @(/1/0/1/2/2) */
        case TERMINATE_SIG: {
            BSP_terminate(0);
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

