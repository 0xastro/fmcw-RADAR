/**
 *   @file  gtrack_math.c
 *
 *   @brief
 *      This is a set of matrix functions used by GTRACK Algorithm
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <math.h>

#include <ti/alg/gtrack/gtrack.h>
#include <ti/alg/gtrack/include/gtrack_int.h>

static const float cosTable[360] = {     
  -0.999847695156391270f, -0.999390827019095760f, -0.998629534754573830f,     
  -0.997564050259824200f, -0.996194698091745550f, -0.994521895368273290f,     
  -0.992546151641321980f, -0.990268068741570250f,     
  -0.987688340595137660f, -0.984807753012208020f, -0.981627183447663980f,     
  -0.978147600733805690f, -0.974370064785235250f, -0.970295726275996470f,     
  -0.965925826289068200f, -0.961261695938318670f,     
  -0.956304755963035440f, -0.951056516295153530f, -0.945518575599316740f,     
  -0.939692620785908320f, -0.933580426497201740f, -0.927183854566787310f,     
  -0.920504853452440150f, -0.913545457642600760f,     
  -0.906307787036649940f, -0.898794046299167040f, -0.891006524188367790f,     
  -0.882947592858926770f, -0.874619707139395740f, -0.866025403784438710f,     
  -0.857167300702112220f, -0.848048096156425960f,     
  -0.838670567945424160f, -0.829037572555041620f, -0.819152044288991580f,     
  -0.809016994374947340f, -0.798635510047292940f, -0.788010753606721900f,     
  -0.777145961456970680f, -0.766044443118977900f,     
  -0.754709580222772010f, -0.743144825477394130f, -0.731353701619170460f,     
  -0.719339800338651300f, -0.707106781186547460f, -0.694658370458997030f,     
  -0.681998360062498370f, -0.669130606358858240f,     
  -0.656059028990507500f, -0.642787609686539360f, -0.629320391049837280f,     
  -0.615661475325658290f, -0.601815023152048380f, -0.587785252292473030f,     
  -0.573576436351045830f, -0.559192903470746680f,     
  -0.544639035015027080f, -0.529919264233204790f, -0.515038074910054270f,     
  -0.499999999999999780f, -0.484809620246337000f, -0.469471562785890530f,     
  -0.453990499739546750f, -0.438371146789077510f,     
  -0.422618261740699330f, -0.406736643075800100f, -0.390731128489273600f,     
  -0.374606593415912070f, -0.358367949545300270f, -0.342020143325668710f,     
  -0.325568154457156420f, -0.309016994374947340f,     
  -0.292371704722736660f, -0.275637355816999050f, -0.258819045102520850f,     
  -0.241921895599667790f, -0.224951054343864810f, -0.207911690817759120f,     
  -0.190808995376544800f, -0.173648177666930300f,     
  -0.156434465040231040f, -0.139173100960065350f, -0.121869343405147370f,     
  -0.104528463267653330f, -0.087155742747658235f, -0.069756473744125330f,     
  -0.052335956242943620f, -0.034899496702500733f,     
  -0.017452406437283477f, 0.000000000000000061f, 0.017452406437283376f,     
  0.034899496702501080f, 0.052335956242943966f, 0.069756473744125455f,     
  0.087155742747658138f, 0.104528463267653460f,     
  0.121869343405147490f, 0.139173100960065690f, 0.156434465040230920f,     
  0.173648177666930410f, 0.190808995376544920f, 0.207911690817759450f,     
  0.224951054343864920f, 0.241921895599667900f,     
  0.258819045102520740f, 0.275637355816999160f, 0.292371704722736770f,     
  0.309016994374947450f, 0.325568154457156760f, 0.342020143325668820f,     
  0.358367949545300380f, 0.374606593415911960f,     
  0.390731128489273940f, 0.406736643075800210f, 0.422618261740699440f,     
  0.438371146789077460f, 0.453990499739546860f, 0.469471562785890860f,     
  0.484809620246337110f, 0.500000000000000110f,     
  0.515038074910054380f, 0.529919264233204900f, 0.544639035015027200f,     
  0.559192903470746790f, 0.573576436351046050f, 0.587785252292473140f,     
  0.601815023152048270f, 0.615661475325658290f,     
  0.629320391049837500f, 0.642787609686539360f, 0.656059028990507280f,     
  0.669130606358858240f, 0.681998360062498480f, 0.694658370458997370f,     
  0.707106781186547570f, 0.719339800338651190f,     
  0.731353701619170570f, 0.743144825477394240f, 0.754709580222772010f,     
  0.766044443118978010f, 0.777145961456970900f, 0.788010753606722010f,     
  0.798635510047292830f, 0.809016994374947450f,     
  0.819152044288991800f, 0.829037572555041620f, 0.838670567945424050f,     
  0.848048096156425960f, 0.857167300702112330f, 0.866025403784438710f,     
  0.874619707139395740f, 0.882947592858926990f,     
  0.891006524188367900f, 0.898794046299167040f, 0.906307787036649940f,     
  0.913545457642600870f, 0.920504853452440370f, 0.927183854566787420f,     
  0.933580426497201740f, 0.939692620785908430f,     
  0.945518575599316850f, 0.951056516295153530f, 0.956304755963035440f,     
  0.961261695938318890f, 0.965925826289068310f, 0.970295726275996470f,     
  0.974370064785235250f, 0.978147600733805690f,     
  0.981627183447663980f, 0.984807753012208020f, 0.987688340595137770f,     
  0.990268068741570360f, 0.992546151641321980f, 0.994521895368273290f,     
  0.996194698091745550f, 0.997564050259824200f,     
  0.998629534754573830f, 0.999390827019095760f, 0.999847695156391270f,     
  1.000000000000000000f, 0.999847695156391270f, 0.999390827019095760f,     
  0.998629534754573830f, 0.997564050259824200f,     
  0.996194698091745550f, 0.994521895368273290f, 0.992546151641321980f,     
  0.990268068741570360f, 0.987688340595137770f, 0.984807753012208020f,     
  0.981627183447663980f, 0.978147600733805690f,     
  0.974370064785235250f, 0.970295726275996470f, 0.965925826289068310f,     
  0.961261695938318890f, 0.956304755963035440f, 0.951056516295153530f,     
  0.945518575599316850f, 0.939692620785908430f,     
  0.933580426497201740f, 0.927183854566787420f, 0.920504853452440370f,     
  0.913545457642600870f, 0.906307787036649940f, 0.898794046299167040f,     
  0.891006524188367900f, 0.882947592858926990f,     
  0.874619707139395740f, 0.866025403784438710f, 0.857167300702112330f,     
  0.848048096156425960f, 0.838670567945424050f, 0.829037572555041620f,     
  0.819152044288991800f, 0.809016994374947450f,     
  0.798635510047292830f, 0.788010753606722010f, 0.777145961456970900f,     
  0.766044443118978010f, 0.754709580222772010f, 0.743144825477394240f,     
  0.731353701619170570f, 0.719339800338651190f,     
  0.707106781186547570f, 0.694658370458997370f, 0.681998360062498480f,     
  0.669130606358858240f, 0.656059028990507280f, 0.642787609686539360f,     
  0.629320391049837500f, 0.615661475325658290f,     
  0.601815023152048270f, 0.587785252292473140f, 0.573576436351046050f,     
  0.559192903470746790f, 0.544639035015027200f, 0.529919264233204900f,     
  0.515038074910054380f, 0.500000000000000110f,     
  0.484809620246337110f, 0.469471562785890860f, 0.453990499739546860f,     
  0.438371146789077460f, 0.422618261740699440f, 0.406736643075800210f,     
  0.390731128489273940f, 0.374606593415911960f,     
  0.358367949545300380f, 0.342020143325668820f, 0.325568154457156760f,     
  0.309016994374947450f, 0.292371704722736770f, 0.275637355816999160f,     
  0.258819045102520740f, 0.241921895599667900f,     
  0.224951054343864920f, 0.207911690817759450f, 0.190808995376544920f,     
  0.173648177666930410f, 0.156434465040230920f, 0.139173100960065690f,     
  0.121869343405147490f, 0.104528463267653460f,     
  0.087155742747658138f, 0.069756473744125455f, 0.052335956242943966f,     
  0.034899496702501080f, 0.017452406437283376f, 0.000000000000000061f,     
  -0.017452406437283477f, -0.034899496702500733f,     
  -0.052335956242943620f, -0.069756473744125330f, -0.087155742747658235f,     
  -0.104528463267653330f, -0.121869343405147370f, -0.139173100960065350f,     
  -0.156434465040231040f, -0.173648177666930300f,     
  -0.190808995376544800f, -0.207911690817759120f, -0.224951054343864810f,     
  -0.241921895599667790f, -0.258819045102520850f, -0.275637355816999050f,     
  -0.292371704722736660f, -0.309016994374947340f,     
  -0.325568154457156420f, -0.342020143325668710f, -0.358367949545300270f,     
  -0.374606593415912070f, -0.390731128489273600f, -0.406736643075800100f,     
  -0.422618261740699330f, -0.438371146789077510f,     
  -0.453990499739546750f, -0.469471562785890530f, -0.484809620246337000f,     
  -0.499999999999999780f, -0.515038074910054270f, -0.529919264233204790f,     
  -0.544639035015027080f, -0.559192903470746680f,     
  -0.573576436351045830f, -0.587785252292473030f, -0.601815023152048380f,     
  -0.615661475325658290f, -0.629320391049837280f, -0.642787609686539360f,     
  -0.656059028990507500f, -0.669130606358858240f,     
  -0.681998360062498370f, -0.694658370458997030f, -0.707106781186547460f,     
  -0.719339800338651300f, -0.731353701619170460f, -0.743144825477394130f,     
  -0.754709580222772010f, -0.766044443118977900f,     
  -0.777145961456970680f, -0.788010753606721900f, -0.798635510047292940f,     
  -0.809016994374947340f, -0.819152044288991580f, -0.829037572555041620f,     
  -0.838670567945424160f, -0.848048096156425960f,     
  -0.857167300702112220f, -0.866025403784438710f, -0.874619707139395740f,     
  -0.882947592858926770f, -0.891006524188367790f, -0.898794046299167040f,     
  -0.906307787036649940f, -0.913545457642600760f,     
  -0.920504853452440150f, -0.927183854566787310f, -0.933580426497201740f,     
  -0.939692620785908320f, -0.945518575599316740f, -0.951056516295153530f,     
  -0.956304755963035440f, -0.961261695938318670f,     
  -0.965925826289068200f, -0.970295726275996470f, -0.974370064785235250f,     
  -0.978147600733805690f, -0.981627183447663980f, -0.984807753012208020f,     
  -0.987688340595137660f, -0.990268068741570250f,     
  -0.992546151641321980f, -0.994521895368273290f, -0.996194698091745550f,     
  -0.997564050259824200f, -0.998629534754573830f, -0.999390827019095760f,     
  -0.999847695156391270f, -1.000000000000000000f     
};    
     
static const float sinTable[360] = {     
  -0.017452406437283439f, -0.034899496702500699f, -0.052335956242943807f,     
  -0.069756473744125524f, -0.087155742747658638f, -0.104528463267653730f,     
  -0.121869343405147550f, -0.139173100960065740f,     
  -0.156434465040230980f, -0.173648177666930280f, -0.190808995376544970f,     
  -0.207911690817759310f, -0.224951054343864780f, -0.241921895599667730f,     
  -0.258819045102521020f, -0.275637355816999660f,     
  -0.292371704722737050f, -0.309016994374947510f, -0.325568154457156980f,     
  -0.342020143325668880f, -0.358367949545300210f, -0.374606593415912240f,     
  -0.390731128489274160f, -0.406736643075800430f,     
  -0.422618261740699500f, -0.438371146789077290f, -0.453990499739546860f,     
  -0.469471562785891080f, -0.484809620246337170f, -0.499999999999999940f,     
  -0.515038074910054380f, -0.529919264233204900f,     
  -0.544639035015026860f, -0.559192903470746900f, -0.573576436351046380f,     
  -0.587785252292473250f, -0.601815023152048160f, -0.615661475325658400f,     
  -0.629320391049837720f, -0.642787609686539470f,     
  -0.656059028990507280f, -0.669130606358858350f, -0.681998360062498590f,     
  -0.694658370458997140f, -0.707106781186547570f, -0.719339800338651410f,     
  -0.731353701619170570f, -0.743144825477394240f,     
  -0.754709580222771790f, -0.766044443118978010f, -0.777145961456971010f,     
  -0.788010753606722010f, -0.798635510047292720f, -0.809016994374947450f,     
  -0.819152044288992020f, -0.829037572555041740f,     
  -0.838670567945424050f, -0.848048096156426070f, -0.857167300702112330f,     
  -0.866025403784438710f, -0.874619707139395850f, -0.882947592858927100f,     
  -0.891006524188367900f, -0.898794046299166930f,     
  -0.906307787036650050f, -0.913545457642600980f, -0.920504853452440370f,     
  -0.927183854566787420f, -0.933580426497201740f, -0.939692620785908430f,     
  -0.945518575599316850f, -0.951056516295153640f,     
  -0.956304755963035550f, -0.961261695938318890f, -0.965925826289068310f,     
  -0.970295726275996470f, -0.974370064785235250f, -0.978147600733805690f,     
  -0.981627183447663980f, -0.984807753012208020f,     
  -0.987688340595137660f, -0.990268068741570360f, -0.992546151641322090f,     
  -0.994521895368273400f, -0.996194698091745550f, -0.997564050259824200f,     
  -0.998629534754573830f, -0.999390827019095760f,     
  -0.999847695156391270f, -1.000000000000000000f, -0.999847695156391270f,     
  -0.999390827019095760f, -0.998629534754573830f, -0.997564050259824200f,     
  -0.996194698091745550f, -0.994521895368273290f,     
  -0.992546151641321980f, -0.990268068741570250f, -0.987688340595137770f,     
  -0.984807753012208020f, -0.981627183447663980f, -0.978147600733805580f,     
  -0.974370064785235250f, -0.970295726275996470f,     
  -0.965925826289068310f, -0.961261695938318890f, -0.956304755963035440f,     
  -0.951056516295153530f, -0.945518575599316740f, -0.939692620785908320f,     
  -0.933580426497201740f, -0.927183854566787420f,     
  -0.920504853452440260f, -0.913545457642600870f, -0.906307787036649940f,     
  -0.898794046299167040f, -0.891006524188367790f, -0.882947592858926880f,     
  -0.874619707139395740f, -0.866025403784438600f,     
  -0.857167300702112220f, -0.848048096156426070f, -0.838670567945423940f,     
  -0.829037572555041740f, -0.819152044288991800f, -0.809016994374947450f,     
  -0.798635510047292830f, -0.788010753606722010f,     
  -0.777145961456970790f, -0.766044443118978010f, -0.754709580222772010f,     
  -0.743144825477394240f, -0.731353701619170460f, -0.719339800338651080f,     
  -0.707106781186547460f, -0.694658370458997250f,     
  -0.681998360062498480f, -0.669130606358858240f, -0.656059028990507160f,     
  -0.642787609686539250f, -0.629320391049837390f, -0.615661475325658180f,     
  -0.601815023152048270f, -0.587785252292473140f,     
  -0.573576436351046050f, -0.559192903470746900f, -0.544639035015027080f,     
  -0.529919264233204900f, -0.515038074910054160f, -0.499999999999999940f,     
  -0.484809620246337060f, -0.469471562785890810f,     
  -0.453990499739546750f, -0.438371146789077400f, -0.422618261740699440f,     
  -0.406736643075800150f, -0.390731128489273720f, -0.374606593415912010f,     
  -0.358367949545300270f, -0.342020143325668710f,     
  -0.325568154457156640f, -0.309016994374947400f, -0.292371704722736770f,     
  -0.275637355816999160f, -0.258819045102520740f, -0.241921895599667730f,     
  -0.224951054343865000f, -0.207911690817759310f,     
  -0.190808995376544800f, -0.173648177666930330f, -0.156434465040230870f,     
  -0.139173100960065440f, -0.121869343405147480f, -0.104528463267653460f,     
  -0.087155742747658166f, -0.069756473744125302f,     
  -0.052335956242943828f, -0.034899496702500969f, -0.017452406437283512f,     
  0.000000000000000000f, 0.017452406437283512f, 0.034899496702500969f,     
  0.052335956242943828f, 0.069756473744125302f,     
  0.087155742747658166f, 0.104528463267653460f, 0.121869343405147480f,     
  0.139173100960065440f, 0.156434465040230870f, 0.173648177666930330f,     
  0.190808995376544800f, 0.207911690817759310f,     
  0.224951054343865000f, 0.241921895599667730f, 0.258819045102520740f,     
  0.275637355816999160f, 0.292371704722736770f, 0.309016994374947400f,     
  0.325568154457156640f, 0.342020143325668710f,     
  0.358367949545300270f, 0.374606593415912010f, 0.390731128489273720f,     
  0.406736643075800150f, 0.422618261740699440f, 0.438371146789077400f,     
  0.453990499739546750f, 0.469471562785890810f,     
  0.484809620246337060f, 0.499999999999999940f, 0.515038074910054160f,     
  0.529919264233204900f, 0.544639035015027080f, 0.559192903470746900f,     
  0.573576436351046050f, 0.587785252292473140f,     
  0.601815023152048270f, 0.615661475325658180f, 0.629320391049837390f,     
  0.642787609686539250f, 0.656059028990507160f, 0.669130606358858240f,     
  0.681998360062498480f, 0.694658370458997250f,     
  0.707106781186547460f, 0.719339800338651080f, 0.731353701619170460f,     
  0.743144825477394240f, 0.754709580222772010f, 0.766044443118978010f,     
  0.777145961456970790f, 0.788010753606722010f,     
  0.798635510047292830f, 0.809016994374947450f, 0.819152044288991800f,     
  0.829037572555041740f, 0.838670567945423940f, 0.848048096156426070f,     
  0.857167300702112220f, 0.866025403784438600f,     
  0.874619707139395740f, 0.882947592858926880f, 0.891006524188367790f,     
  0.898794046299167040f, 0.906307787036649940f, 0.913545457642600870f,     
  0.920504853452440260f, 0.927183854566787420f,     
  0.933580426497201740f, 0.939692620785908320f, 0.945518575599316740f,     
  0.951056516295153530f, 0.956304755963035440f, 0.961261695938318890f,     
  0.965925826289068310f, 0.970295726275996470f,     
  0.974370064785235250f, 0.978147600733805580f, 0.981627183447663980f,     
  0.984807753012208020f, 0.987688340595137770f, 0.990268068741570250f,     
  0.992546151641321980f, 0.994521895368273290f,     
  0.996194698091745550f, 0.997564050259824200f, 0.998629534754573830f,     
  0.999390827019095760f, 0.999847695156391270f, 1.000000000000000000f,     
  0.999847695156391270f, 0.999390827019095760f,     
  0.998629534754573830f, 0.997564050259824200f, 0.996194698091745550f,     
  0.994521895368273400f, 0.992546151641322090f, 0.990268068741570360f,     
  0.987688340595137660f, 0.984807753012208020f,     
  0.981627183447663980f, 0.978147600733805690f, 0.974370064785235250f,     
  0.970295726275996470f, 0.965925826289068310f, 0.961261695938318890f,     
  0.956304755963035550f, 0.951056516295153640f,     
  0.945518575599316850f, 0.939692620785908430f, 0.933580426497201740f,     
  0.927183854566787420f, 0.920504853452440370f, 0.913545457642600980f,     
  0.906307787036650050f, 0.898794046299166930f,     
  0.891006524188367900f, 0.882947592858927100f, 0.874619707139395850f,     
  0.866025403784438710f, 0.857167300702112330f, 0.848048096156426070f,     
  0.838670567945424050f, 0.829037572555041740f,     
  0.819152044288992020f, 0.809016994374947450f, 0.798635510047292720f,     
  0.788010753606722010f, 0.777145961456971010f, 0.766044443118978010f,     
  0.754709580222771790f, 0.743144825477394240f,     
  0.731353701619170570f, 0.719339800338651410f, 0.707106781186547570f,     
  0.694658370458997140f, 0.681998360062498590f, 0.669130606358858350f,     
  0.656059028990507280f, 0.642787609686539470f,     
  0.629320391049837720f, 0.615661475325658400f, 0.601815023152048160f,     
  0.587785252292473250f, 0.573576436351046380f, 0.559192903470746900f,     
  0.544639035015026860f, 0.529919264233204900f,     
  0.515038074910054380f, 0.499999999999999940f, 0.484809620246337170f,     
  0.469471562785891080f, 0.453990499739546860f, 0.438371146789077290f,     
  0.422618261740699500f, 0.406736643075800430f,     
  0.390731128489274160f, 0.374606593415912240f, 0.358367949545300210f,     
  0.342020143325668880f, 0.325568154457156980f, 0.309016994374947510f,     
  0.292371704722737050f, 0.275637355816999660f,     
  0.258819045102521020f, 0.241921895599667730f, 0.224951054343864780f,     
  0.207911690817759310f, 0.190808995376544970f, 0.173648177666930280f,     
  0.156434465040230980f, 0.139173100960065740f,     
  0.121869343405147550f, 0.104528463267653730f, 0.087155742747658638f,     
  0.069756473744125524f, 0.052335956242943807f, 0.034899496702500699f,     
  0.017452406437283439f, 0.000000000000000122f     
};     

/**
*  @b Description
*  @n
*		This function is used to compute sine and cosine of the angle in degrees
*
*  @param[in]  theta
*		Angle, in degrees
*		Matrix B
*  @param[out] pSinVal
*		pointer to computed sine value, degrees
*  @param[out] pCosVal
*		pointer to computed sine value, degrees
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_sincosd(float theta, float * pSinVal, float * pCosVal)     
{     
    uint32_t i;                                /* Index for reading nearwst output values */     
    float x1 = -179.0f;                        /* Initial input value */     
    float ysin0, ysin1;                        /* nearest output values */     
    float ycos0, ycos1;                        /* nearest output values */     
    float fract;                               /* fractional part of input */     
    float ftheta = (float)((int32_t)theta);  


    /* index calculation for reading nearest output values */     
    i = (uint32_t) (theta - x1);     

    /* Calculation of fractional part */     
    if(theta > 0.0f)     
    {     
        fract = theta - ftheta;     
    }     
    else     
    {     
        fract = (theta - ftheta) + 1.0f;     
    }     

    /* reading nearest sine output values */     
    ysin0 = sinTable[i];     
    ysin1 = sinTable[i + 1u];     

    /* reading nearest cosine output values */     
    ycos0 = cosTable[i];     
    ycos1 = cosTable[i + 1u];     

    /* difference of nearest sine output value */  
    ysin1 = ysin1 - ysin0;  

    /* difference of nearest cosine output value */  
    ycos1 = ycos1 - ycos0;  

    /* Calculation of sine value */     
    *pSinVal = ysin0 + (fract * ysin1);

    /* Calculation of cosine value */     
    *pCosVal = ycos0 + (fract * ycos1);
}     


/**
*  @b Description
*  @n
*		This function is used to create identity matrix 
*
*  @param[in]  size
*		Size of identity matrix
*  @param[out] A
*		Matrix A(size,size) = eye(size)
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixEye(uint16_t size, float *A)
{
	/* A(size*size) = eye(size) */
	uint16_t i;

	for (i = 0U; i < size*size; i++)
	{
		A[i] = 0.f;
	}

	for (i = 0U; i < size; i++)
	{
        A[i+i*size] = 1.0f;
	}
}

/**
*  @b Description
*  @n
*		This function is used to initialise matrix to a value 
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  value
*		value to set
*  @param[out] A
*		Matrix A(rows,cols) = ones(rows,cols)*value
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixInit(uint16_t rows, uint16_t cols, float value, float *A)
{
	/* A(rows*cols) = ones(rows,cols)*value */
	uint16_t i;

	for (i = 0U; i < rows*cols; i++)
	{
		A[i] = value;
	}
}

/**
*  @b Description
*  @n
*		This function is used to create diagonal square matrix 
*
*  @param[in]  size
*		Size of square matrix
*  @param[in]  v
*		Diagonal vector
*  @param[out] A
*		Matrix A(size,size) = diag(v(size))
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixSetDiag(uint16_t size, float *v, float *A)
{
	/* A(size*size) = diag(v(size)) */
	uint16_t i;

	for (i = 0U; i < size*size; i++)
	{
		A[i] = 0;
	}

	for (i = 0U; i < size; i++)
	{
        A[i+i*size] = v[i];
	}
}

/**
*  @b Description
*  @n
*		This function is used to get diagonal from the square matrix 
*
*  @param[in]  size
*		Size of square matrix
*  @param[in] A
*		Matrix A(size,size)
*  @param[out]  v
*		Diagonal vector, v(size) = diag(A(size*size))
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixGetDiag(uint16_t size, float *A, float *v)
{
	/* v(size) = diag(A(size*size)) */
	uint16_t i;
	for (i = 0U; i < size; i++)
	{
        v[i] = A[i+i*size];
	}
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  rows
*		Outer dimension, number of rows
*  @param[in]  m
*		Inner dimension
*  @param[in]  cols
*		Outer dimension, number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(rows,cols) = A(rows,m) X B(cols,m)T
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixMultiply(uint16_t rows, uint16_t m, uint16_t cols, float *A, float *B, float *C)
{
	/* C(rows*cols) = A(rows*m)*B(m*cols) */
	uint16_t i,j, k;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{			
			C[i*cols + j] = 0;
			for (k = 0; k < m; k++)
			{
				C[i*cols+j] += A[i*m+k] * B[k*cols + j];
			}
		}
	}
}

/**
*  @b Description
*  @n
*		This function is used to multiply two matrices. Second Matrix is getting transposed first
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  rows
*		Outer dimension, number of rows
*  @param[in]  m
*		Inner dimension
*  @param[in]  cols
*		Outer dimension, number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(rows,cols) = A(rows,m) X B(cols,m)T
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixTransposeMultiply(uint16_t rows, uint16_t m, uint16_t cols, float *A, float *B, float *C)
{
	/* C(rows*cols) = A(rows*m)*B(cols*m)T */
	uint16_t i,j, k;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{			
			C[i*cols + j] = 0;
			for (k = 0; k < m; k++)
			{
				C[i*cols+j] += A[i*m+k] * B[k + j*m];
			}
		}
	}
}

/**
*  @b Description
*  @n
*		This function is used to multiply matrix by a scalar.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  c
*		Scalar c
*  @param[out]  B
*		Matrix B(rows,cols) = A(rows,cols) * c
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixScalarMultiply(uint16_t rows, uint16_t cols, float *A, float c, float *B)
{
	/* B(rows*cols) = A(rows*cols)*C */
	uint16_t i;
	for (i = 0U; i < rows*cols; i++)
	{
		B[i] = A[i] * c;
	}
}

/**
*  @b Description
*  @n
*		This function is used to add two matrices.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(rows,cols) = A(rows,cols) + B(rows,cols)
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixAdd(uint16_t rows, uint16_t cols, float *A, float *B, float *C)
{
	/* C(rows*cols) = A(rows*cols) + B(rows*cols) */
	uint16_t i;
	for (i = 0U; i < rows*cols; i++)
	{
		C[i] = A[i] + B[i];
	}
}

/**
*  @b Description
*  @n
*		This function is used to subtract two matrices.
*		Matrices are all real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[out]  C
*		Matrix C(rows,cols) = A(rows,cols) - B(rows,cols)
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixSub(uint16_t rows, uint16_t cols, float *A, float *B, float *C)
{
	/* C(rows*cols) = A(rows*cols) - B(rows*cols) */
	uint16_t i;
	for (i = 0U; i < rows*cols; i++)
	{
		C[i] = A[i] - B[i];
	}
}

/**
*  @b Description
*  @n
*		This function is used to force matrix symmetry by averaging off-diagonal elements
*		Matrices are squared, real, single precision floating point.
*		Matrices are in row-major order
*
*  @param[in]  m (m=rows=cols)
*		Number of rows and cols
*  @param[in]  A
*		Matrix A
*  @param[out]  B
*		Matrix B
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixMakeSymmetrical(uint16_t m, float *A, float *B)
{
	/* Make square matrix symmetrical by averaging off-diagonal elements */
	uint16_t i,j;
	for (i = 0U; i < m-1; i++)
	{
		B[i*m + i] = A[i*m + i];
		for (j = i+1; j < m; j++)
		{
			B[i*m+j] = B[j*m+i] = 0.5f*(A[i*m+j]+A[j*m+i]);
		}
	}
	B[i*m + i] = A[i*m + i];
}

/**
*  @b Description
*  @n
*		This function performs cholesky decomposition of square matrix.
*		Matrix are squared, real, single precision floating point.
*		Matrix are in row-major order
*
*  @param[in]  dim (dim=rows=cols)
*		Number of rows and cols
*  @param[in]  A
*		Matrix A
*  @param[out]  G
*		Matrix G = cholseky(A);
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixCholesky(uint16_t dim, const float *A, float *G)
{
    float v[4] = {0.f};
	float temp;
    uint16_t i, j, k;

    for (i = 0U; i < dim*dim; i++)
	{
        G[i] = 0;
	}

    for (j = 0U; j < dim; j++)
    {
		/* v(j:n,1) = A(j:n,j) */
		for (i = j; i < dim; i++)
		{
		    v[i] = A[i * dim + j];
        }
		
		for (k = 0U; k < j; k++)
        {
			/* v(j:n,1) = v(j:n,1) - G(j,k)*G(j:n,k) */
			for (i = j; i < dim; i++)
			{
				v[i] = v[i] - G[j*dim + k] * G[i*dim + k];
			}
		}
    
        /* G(j:n,j) = v(j:n,1)/sqrt(v(j)) */
		temp = 1.0f/sqrtf(v[j]);
		for (i = j; i < dim; i++)
		{
			G[i*dim + j] = v[i]*temp;
		}
	}
}

/**
*  @b Description
*  @n
*		This function is used to initialise vector to a value 
*
*  @param[in]  size
*		Size of vector
*  @param[in]  value
*		value to set
*  @param[out]  A
*		Vector A
*		
*       A(size) = ones(size,1)*value
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorInit(uint16_t size, float value, float *A)
{
	/* A(size) = ones(size,1)*value */
	uint16_t i;

	for (i = 0U; i < size; i++)
	{
		A[i] = value;
	}
}
/**
*  @b Description
*  @n
*		This function adds two vectors
*		Vectors are real, single precision floating point.
*
*  @param[in]  size
*		Size of vector
*  @param[in]  A
*		Vector A
*  @param[in]  B
*		Vector B
*  @param[out]  C
*		Vector C = A + B;
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorAdd(uint16_t size, float *A, float *B, float *C)
{
	uint16_t i;
	for (i = 0U; i < size; i++)
	{
        C[i] = A[i] + B[i];
	}
}

/**
*  @b Description
*  @n
*		This function subtracts two vectors
*		Vectors are real, single precision floating point.
*
*  @param[in]  size
*		Size of vectors
*  @param[in]  A
*		Vector A
*  @param[in]  B
*		Vector B
*  @param[out]  C
*		Vector C = A - B;
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorSub(uint16_t size, float *A, float *B, float *C)
{
	uint16_t i;
	for (i = 0U; i < size; i++)
	{
        C[i] = A[i] - B[i];
	}
}

/**
*  @b Description
*  @n
*		This function multiplies vector by scalar
*		Vectors are real, single precision floating point.
*		Scalar is real, single precision floating point.
*
*  @param[in]  size
*		Size of vector
*  @param[in]  A
*		Vector A
*  @param[in]  c
*		Scalar c
*  @param[out]  B
*		Vector B = A*c;
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorScalarMul(uint16_t size, float *A, float c, float *B)
{	
    uint16_t i;
	for (i = 0U; i < size; i++)
	{
        B[i] = A[i]*c;
	}
}

/**
*  @b Description
*  @n
*		This function performs multiplies vector by scalar and accumulates the results
*		Vectors are real, single precision floating point.
*		Scalar is real, single precision floating point.
*
*  @param[in]  size
*		Size of vector
*  @param[in]  A
*		Vector A
*  @param[in]  c
*		Scalar c
*  @param[in, out]  B
*		Vector B = B + A*c;
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorScalarMulAcc(uint16_t size, float *A, float c, float *B)
{	
    uint16_t i;
	for (i = 0U; i < size; i++)
	{
        B[i] = B[i] + A[i]*c;
	}
}

/**
*  @b Description
*  @n
*		This function performs IIR vector filtering
*		Vectors are real, single precision floating point.
*		Alpha is real, single precision floating point.
*
*  @param[in]  size
*		Size of vector
*  @param[in, out]  A
*		Vector A
*  @param[in]  alpha
*		Weighting factor for new information, (0<=alpha<=1.0f)
*  @param[in]  B
*		New information vector B
*
*		Vector A = A*(1.0f-alpha) + B*alpha;
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_vectorFilter(uint16_t size, float *A, float alpha, float *B)
{	
    uint16_t i;
	for (i = 0U; i < size; i++)
	{
        A[i] = A[i]*(1.0f - alpha) + B[i]*alpha;
	}
}


/**
*  @b Description
*  @n
*		This function accumulates covariance matrix with variances from input vector and mean
*		Matrices are all real, single precision floating point.
*		Vectors are real, single precision floating point.
*
*  @param[in]  size
*		Size of square matrix
*  @param[in]  A
*		Matrix A
*  @param[in]  v
*		Vector v
*  @param[in]  mean
*		Vector mean
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixCovAcc(uint16_t size, float *A, float *v, float *mean)
{
	uint16_t i,j;
    float d1, d2;

	for (i = 0U; i < size; i++) 
	{
        d1 = v[i]-mean[i];
        for (j = i; j < size; j++) 
		{
            d2 = v[j]-mean[j];
            A[i*size+j] += d1*d2;
        }
    }
}

/**
*  @b Description
*  @n
*		This function normalizes covariance matrix
*		Matrices are all real, single precision floating point.
*
*  @param[in]  size
*		Size of square matrix
*  @param[in,out]  A
*		Matrix A
*  @param[in]  num
*		Number of measurments num
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixCovNormalize(uint16_t size, float *A, uint16_t num)
{
	uint16_t i,j;
	for (i = 0U; i < size; i++) 
	{
        A[i*size+i] /= num;
        for (j = i+1; j < size; j++) 
		{
            A[i*size+j] /= num;
            A[i+j*size] = A[i*size+j];
        }
    }
}        
/**
*  @b Description
*  @n
*		This function filters covariance matrix
*		Matrices are all real, single precision floating point.
*
*  @param[in]  size
*		Size of square matrix
*  @param[in,out]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*  @param[in]  alpha
*		Filtering coefficient alpha
*  Matrix A = (1-alpha)*A + alpha*B
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/

void gtrack_matrixCovFilter(uint16_t size, float *A, float *B, float alpha)
{
	uint16_t i,j;
	for (i = 0U; i < size; i++) 
	{
        A[i*size+i] = (1-alpha)*A[i*size+i] + alpha*B[i*size+i];
        for (j = i+1; j < size; j++) 
		{
            A[i*size+j] = (1-alpha)*A[i*size+j] + alpha*B[i*size+j];
            A[i+j*size] = A[i*size+j];
        }
    }
}        

/**
*  @b Description
*  @n
*		This function prints a matrix
*		Matrices are all real, single precision floating point.
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  A
*		Matrix A
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixPrint(uint16_t rows, uint16_t cols, float *A)
{
	uint16_t i,j;
	for (i = 0U; i < rows; i++)
	{
		for (j = 0U; j < cols-1; j++)
		{
			gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.4f\t", A[i*cols +j]); 
		}
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.4f\n", A[i*cols +j]);
	}
	gtrack_log(GTRACK_VERBOSE_DEBUG, "\n");
}
/**
*  @b Description
*  @n
*		This function prints 2 matrices side by side
*		Matrices are all real, single precision floating point.
*
*  @param[in]  rows
*		Number of rows
*  @param[in]  cols
*		Number of cols
*  @param[in]  A
*		Matrix A
*  @param[in]  B
*		Matrix B
*
*  \ingroup GTRACK_ALG_MATH_FUNCTION
*
*  @retval
*      None
*/
void gtrack_matrixPrint2(uint16_t rows, uint16_t cols, float *A, float *B)
{
	/* print 2 matrices side by side */
	uint16_t i,j;
	for (i = 0U; i < rows; i++)
	{
		for (j = 0U; j < cols-1; j++)
		{
			gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.2f\t",A[i*cols +j]); 
		}
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.2f\t\t",A[i*cols +j]);

		for (j = 0U; j < cols-1; j++)
		{
			gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.2f\t",B[i*cols +j]); 
		}
		gtrack_log(GTRACK_VERBOSE_DEBUG, "%6.2f\n",B[i*cols +j]);
	}
	gtrack_log(GTRACK_VERBOSE_DEBUG, "\n");
}
