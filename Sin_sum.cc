// Projet SINUSSUM Fouet Axel SCIPER 375085 version 16.12.2023
#include <iostream>
#include <iomanip>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
using namespace std;

const double EPSIL_DICHO(1e-9);
const double EPSIL_T(1e-13);
// Error messages
const string BAD_SIGNAL("Error: signal type must be SAWTOOTH, SQUARE or TRIANGLE");
const string NBN_TOO_SMALL("Error: the number of sine terms must be greater than 0");
const string NBL_TOO_SMALL("Error: the number of lines must be greater than 2");
const string TIME_MIN_MAX("Error: time max is not bigger than min");
const string SIGNAL_MIN_MAX("Error: signal max is not bigger than min");
const string WRONG_TIME_VAL("Error: both time values must belong to [0., 1.]");
const string NBL_NOT_ODD("Error: the number of lines must be odd");

struct parametre;

typedef vector<vector<char>> affichage;

struct parametre{
/* contient toutes les données d'entrées et les paramètre qui en découle utile pour
le programe*/
	string forme;
	int nb_n;
	double tmin ;
	double tmax ;
	double min ;
	double max ;
	unsigned int nb_l ;
	double delta_t;
	double delta_s;
	unsigned int nb_c; 
	};
///Tache 1
//saisis les valeur d'entrée:
parametre saisie_val();

//verifie si les donnèes sont rentrées correctement:
void detec_erreur(parametre param);
void print_error(string message);

///Tache 2
//Calcule des valeur approcher du signal pour chaque type de signal:
double sum_sawtooth(double t, unsigned int nb_n);
double sum_square(double t, unsigned int nb_n);
double sum_triangle(double t, unsigned int nb_n);

//Calcule des valeur théorique pour chaque type de signal:
double thr_sawtooth(double t);
double thr_square(double t);
double thr_triangle(double t);

//regroupe les autre fonction de calcul du signal:
double calcul_pt_app(double t,parametre param);
double calcul_pt_thr(double t,parametre param);

// Approxime la valeur et sort son bon index a mettre dans le tableau Affichage:
int valeur_aprox(double val,parametre param);

//remplie le tableau affichage avec le signal et l'axe tmp:
affichage remplie(parametre val);
void axe_tmp(affichage& tab,parametre param);
void signal(affichage& tab,parametre param);

//Pour afficher le tableau:
void affiche (affichage tab);
//affiche les lignes avant et après le tableau
void ligne (size_t nbr);

///Tache 3
//Pour definir les bornes de la recherche dichotomique:
void choix_minmax(double &min,double &max,parametre param);

//Pour trouver le max:
double max_dicho(double tmin,double tmax, parametre param);

parametre saisie_val(){
	
	parametre param;

	cin >> param.forme;
	cin >> param.nb_n;
	cin >> param.tmin;
	cin >> param.tmax;
	cin >> param.min;
	cin >> param.max;
	cin >> param.nb_l;
	
	detec_erreur(param);
	
	param.nb_c = 2*param.nb_l-1;
	param.delta_t = (param.tmax-param.tmin)/(param.nb_c-1);
	param.delta_s = (param.max -param.min)/(param.nb_l-1);
	
	return param;
	}

void detec_erreur(parametre param){
	if (param.forme != "SAWTOOTH" and 
		param.forme != "SQUARE" and param.forme != "TRIANGLE"){
		
		print_error(BAD_SIGNAL) ;
		}
	if (param.nb_n <= 0) print_error(NBN_TOO_SMALL) ;
	if (param.tmax < param.tmin) {
		print_error(TIME_MIN_MAX);
		}
	if (param.tmax <0. or param.tmax>1. or param.tmin<0. or param.tmin>1.){
		print_error(WRONG_TIME_VAL) ;
		}
	if (param.max < param.min) print_error(SIGNAL_MIN_MAX) ; 
	if (param.nb_l <= 2) print_error(NBL_TOO_SMALL) ;
	if (param.nb_l %2 == 0) print_error(NBL_NOT_ODD) ;
	}
void print_error(string message) {
    cout << message;
    cout << endl;
    exit(0);
	}
double sum_sawtooth(double t, unsigned int nb_n){
	double res = 0;
	for(unsigned int i(1); i <= nb_n; i++){
		res += (pow((-1.),i)*sin(2*M_PI*i*(t-0.5))/i);

		}
	res *= ((-2)/M_PI);
	return res;
	}
double sum_square(double t, unsigned int nb_n){
	double res = 0;
	for(unsigned int i(1); i <=  nb_n; i++){
		res += (sin(2*M_PI*(2*i-1)*t)/(2*i-1));
		}
	res *= (4/M_PI);
	return res;
	}
double sum_triangle(double t, unsigned int nb_n){
	double res = 0;
	for(unsigned int i(1); i <=  nb_n; i++){
		res +=  (pow((-1.),i)*sin(2*M_PI*(2*i-1)*(t-0.25))/pow((2*i-1),2));
		}
	res *= ((-8)/pow(M_PI,2));
	return res;
	}
double thr_sawtooth(double t){
	if (t+EPSIL_T>=1 or t - EPSIL_T <= 0){
		return NAN; 
		/*NAN valeur retournè par toutes les fonction théarique quand le signal
		présente un discontinuité*/
		}
	return t*2. -1.;
	}
double thr_square(double t){
	if (abs(t-1)<= EPSIL_T or t <= EPSIL_T or abs(t-0.5)<= EPSIL_T){ 
		return NAN;
		}
	if(t<= 0.5){
		return 1.;
		}else{
			return -1.;
			}
	}
double thr_triangle(double t){
	if(t<= 0.5){
		return 4*t-1;
		}else{
			return -4*t+3;
			}
	}


double calcul_pt_app(double t,parametre param){
	//redirige sur la bonne fonction de calcul du signal approché
	if(param.forme == "SAWTOOTH")return sum_sawtooth(t,param.nb_n);
	if(param.forme == "SQUARE")return sum_square(t,param.nb_n);
	if(param.forme == "TRIANGLE")return sum_triangle(t,param.nb_n);
	return NAN;
	}
double calcul_pt_thr(double t,parametre param){
	//redirige sur la bonne fonction de calcul du signal théorique
	if(param.forme == "SAWTOOTH")return thr_sawtooth(t);
	if(param.forme == "SQUARE")return thr_square(t);
	if(param.forme == "TRIANGLE")return thr_triangle(t);
	return NAN;
	}

int valeur_aprox(double val,parametre param){
	int i = (param.max-val)/param.delta_s;
	double j = i +0.5;
	if ((param.max-val)/param.delta_s> j){
		i = i+1;
		}
	return i;
	}
void axe_tmp(affichage& tab,parametre param){

	if (param.max>0 and param.min<0){
		int i =valeur_aprox(0,param);
			
		for(unsigned int j(0); j < param.nb_c;j++){
			tab[i][j] = '.';
			}
		}	
	}
void signal(affichage& tab,parametre param){	
	
	for(unsigned int i(0);i<param.nb_c; i++){
		
		double t =param.tmin+i*param.delta_t;
		double s_thr = calcul_pt_thr(t,param);
		if(!isnan(s_thr)){
		/*la fonction calcul_pt_thr peux retourner nan si le signal présente 
		 une discontinuité au temps t*/
			unsigned int j = valeur_aprox(s_thr,param);
			if(j< tab.size()){ 
				tab[j][i] = '+';
				}
		}
		double s_app = calcul_pt_app(t,param);
		unsigned int k = valeur_aprox(s_app,param);
		if(k< tab.size()){ 
			tab[k][i] = '*';
			}
		}
	}

affichage remplie(parametre param){
	
	affichage tableau (param.nb_l,vector<char>(param.nb_c,' '));
	
	axe_tmp(tableau,param);
	signal(tableau,param);

	return tableau;
	}
	
void ligne (size_t nbr){
	for(size_t i(0); i< nbr;i++){
		cout<<'-';
		}
	cout<<endl;
	}
void affiche (affichage tab){
	ligne(tab[0].size());
	
	for(size_t i(0); i < tab.size();i++){
		for (size_t j(0); j <tab[i].size();j++){
			cout<< tab[i][j];
			}
		cout<<endl;
		}
	ligne(tab[0].size());
	 } 
void choix_minmax(double &min,double &max,parametre param){
	if(param.forme == "SAWTOOTH"){
	max = 1;
	min = 1-1/(2*(double)param.nb_n+1.0);
	}else if (param.forme == "SQUARE"){
		max = 1/(2*(double)param.nb_n+1.0);
		min = 0;	
		}else{ 
			max = 0.5-1/(2*(2*(double)param.nb_n+1.0));
			min = 0.5+1/(2*(2*(double)param.nb_n+1.0));
			}
	}
double max_dicho(double tmin,double tmax,double avant, parametre param){
	
	double tmid = (tmin + tmax)/2;
	double val_mid = calcul_pt_app(tmid,param);
	
	if(abs(val_mid-avant)<EPSIL_DICHO){
		return val_mid;
		}
	
	double delta = tmax-tmin;
	double t1 = tmin +delta/3;
	double t2 = tmin +delta*2/3;
	
	if(calcul_pt_app(t1,param)<calcul_pt_app(t2,param)){
		/* Si la valeur du signal est plus grande en t1 que en t2 alors le max
		se trouve forcement entre la première borne et t2*/
		return max_dicho(t1,tmax,val_mid,param);
		}else{
			return max_dicho(tmin,t2,val_mid,param);
			}
	}
int main(){
///Tache1
parametre param = saisie_val();
///Tache2
affichage tableau = remplie(param);
affiche(tableau);
///Tache 3
double min;
double max;
choix_minmax(min,max,param);

cout<< setprecision(8) << fixed;
cout<<max_dicho(min,max,-2,param)<<endl;
/*donne -2 comme valeur d'avant pour avoir aucun risque de tombé trop proche de la 
première veleur de la dichotomie*/
return 0;
}
