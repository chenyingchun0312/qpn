/*****************************************************************************
* Model: pelican.qm
* File:  ./ped.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/* @(/2/2) .................................................................*/
#include "qpn_port.h"
#include "bsp.h"
#include "pelican.h"

/*Q_DEFINE_THIS_FILE*/

/* Ped class declaration ---------------------------------------------------*/
/* @(/1/1) .................................................................*/
typedef struct PedTag {
/* protected: */
    QMActive super;

/* private: */
    uint8_t retryCtr;
} Ped;

/* protected: */
static QState Ped_initial(Ped * const me);
static QState Ped_wait  (Ped * const me);
static QState Ped_wait_e(Ped * const me);
static QMState const Ped_wait_s = {
    (QMState const *)0,
    Q_STATE_CAST(&Ped_wait),
    Q_ACTION_CAST(0)
};
static QState Ped_off  (Ped * const me);
static QState Ped_off_e(Ped * const me);
static QMState const Ped_off_s = {
    (QMState const *)0,
    Q_STATE_CAST(&Ped_off),
    Q_ACTION_CAST(0)
};


/* Global objects ----------------------------------------------------------*/
Ped AO_Ped; /* the single instance of the Ped AO */

enum PedTimeouts {                     /* various timeouts in ticks */
    N_ATTEMPTS = 10,                   /* number of PED_WAITING attempts */
    WAIT_TOUT = BSP_TICKS_PER_SEC * 3, /* wait between posting PED_WAITING */
    OFF_TOUT  = BSP_TICKS_PER_SEC * 8  /* wait time after posting OFF_SIG */
};

/* Pelican class definition ------------------------------------------------*/
/* @(/1/5) .................................................................*/
void Ped_ctor(void) {
    QMActive_ctor(&AO_Ped.super, Q_STATE_CAST(&Ped_initial));
}
/* @(/1/1) .................................................................*/
/* @(/1/1/1) ...............................................................*/
/* @(/1/1/1/0) */
static QState Ped_initial(Ped * const me) {
    static QActionHandler const act_[] = {
        Q_ACTION_CAST(&Ped_wait_e),
        Q_ACTION_CAST(0)
    };
    return QM_INITIAL(&Ped_wait_s, &act_[0]);
}
/* @(/1/1/1/1) .............................................................*/
static QState Ped_wait_e(Ped * const me) {
    me->retryCtr = N_ATTEMPTS;
    QActive_armX(&me->super, 0U, WAIT_TOUT);
    return QM_ENTRY(&Ped_wait_s);
}
static QState Ped_wait(Ped * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/1/1/0) */
        case Q_TIMEOUT_SIG: {
            --me->retryCtr;
            /* @(/1/1/1/1/0/0) */
            if (me->retryCtr != 0U) {
                QActive_armX((QActive *)me, 0U, WAIT_TOUT);
                QACTIVE_POST((QActive *)&AO_Pelican, PEDS_WAITING_SIG);
                status_ = QM_HANDLED();
            }
            /* @(/1/1/1/1/0/1) */
            else {
                static QActionHandler const act_[] = {
                    Q_ACTION_CAST(&Ped_off_e),
                    Q_ACTION_CAST(0)
                };
                status_ = QM_TRAN(&Ped_off_s, &act_[0]);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/* @(/1/1/1/2) .............................................................*/
static QState Ped_off_e(Ped * const me) {
    QActive_armX(&me->super, 0U, OFF_TOUT);
    QACTIVE_POST((QActive *)&AO_Pelican, OFF_SIG);
    return QM_ENTRY(&Ped_off_s);
}
static QState Ped_off(Ped * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* @(/1/1/1/2/0) */
        case Q_TIMEOUT_SIG: {
            static QActionHandler const act_[] = {
                Q_ACTION_CAST(&Ped_wait_e),
                Q_ACTION_CAST(0)
            };
            QACTIVE_POST((QActive *)&AO_Pelican, ON_SIG);
            status_ = QM_TRAN(&Ped_wait_s, &act_[0]);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
