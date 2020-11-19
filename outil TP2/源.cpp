#include <iostream>
#include <fstream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>

using namespace std;

static int a;
const int NMAX = 30;
const int MMAX = 20;

typedef struct Mp
{
	int m = -1;
	int pc = -1;
	int rc = -1;
};

typedef struct T_INSTANCE {
	int n; //numero de piece
	int m; //numero d'op¨¦ration machine
	int machine[NMAX + 1][MMAX + 1]; //numero de machine [piece][operation]
	int p[NMAX + 1][MMAX + 1];//duree p[piece][operation]
}t_INSTANCE;


typedef struct T_VECTEUR {
	int V[200]; //nb des elements de bierwith
	int ST[NMAX + 1][MMAX + 1] = { 0 };//date
	int cout = 0;
	int PERE[200] = { 0 };// succs
}t_VECTEUR;



void Lecteur(string nom, T_INSTANCE& mon_instance) {
	ifstream infile;
	infile.open(nom);
	infile >> mon_instance.n >> mon_instance.m;
	int sommet = mon_instance.n * mon_instance.m;
	for (int i = 1; i < mon_instance.n + 1; i++)
	{
		for (int j = 1; j < mon_instance.m + 1; j++)
		{
			infile >> mon_instance.machine[i][j];// stocker l'ordre d'utilisation des machines par piece
			infile >> mon_instance.p[i][j]; //stocker la duree de chaque process
		}
	}
	infile.close();

}
void Evaluer(T_INSTANCE& mon_instance, T_VECTEUR& mon_vecteur) {
	int j, mc, d_piece, f_piece;
	mon_vecteur.cout = 0;
	d_piece = 0;
	f_piece = 0;
	mc = 0;
	a = 0;
	int pc, rc;
	int T[NMAX][NMAX];
	Mp mp[MMAX] ;
	pc = rc = -1;
	int* np = new int[mon_instance.n + 1];

	for (int i = 0; i < mon_instance.n+1; i++)
	{
		for (int j = 0; j < mon_instance.m+1; j++)
		{
			mon_vecteur.ST[i][j]=0;
		}
		
	}
	for (int i = 0; i < mon_instance.m*mon_instance.n + 1; i++)
	{
			mon_vecteur.PERE[i] = 0;
	}

	for (int i = 1; i < mon_instance.n + 1; i++)
	{
		np[i] = 0;
	}
	int k = 1;

	for (int i = 1; i < mon_instance.n + 1; i++)
	{
		for (int j = 1; j < mon_instance.m + 1; j++)
		{
			T[i][j] = k;
			k++;
		}
	}
	j = 0;
	int d = 0; // signer la machine qui a la plus long chemin
	for (int i = 1; i < mon_instance.m * mon_instance.n + 1; i++)
	{
		j = mon_vecteur.V[i];
		np[j] = np[j] + 1;
		mc = mon_instance.machine[j][np[j]];
		int a = np[j];
		if (np[j] > 1)
		{
			//prec = pos - 1;
			d_piece = mon_vecteur.ST[j][np[j] - 1];
			f_piece = d_piece + mon_instance.p[j][np[j] - 1];
			if (f_piece > mon_vecteur.ST[j][np[j]])
			{
				mon_vecteur.ST[j][np[j]] = f_piece;
				if (mon_vecteur.ST[j][np[j]] > mon_vecteur.cout)
				{

					mon_vecteur.cout = mon_vecteur.ST[j][np[j]];
					d = mon_instance.machine[j][np[j]];
				}
			}
		}
		if (mp[mc].pc != -1 && mp[mc].rc != -1)
		{
			pc = mp[mc].pc;
			rc = mp[mc].rc;
			mon_vecteur.PERE[(j - 1) * mon_instance.m + np[j]] = T[mp[mc].pc][mp[mc].rc];
			if (mon_vecteur.ST[pc][rc] + mon_instance.p[pc][rc] > mon_vecteur.ST[j][np[j]])
			{
				mon_vecteur.ST[j][np[j]] = mon_vecteur.ST[pc][rc] + mon_instance.p[pc][rc];
				if (mon_vecteur.ST[j][np[j]] > mon_vecteur.cout)
				{
					mon_vecteur.cout = mon_vecteur.ST[j][np[j]];
					d = mon_instance.machine[j][np[j]];
				}
			}
		}
		mp[mc].pc = j;
		mp[mc].rc = np[j];
	}
	int nmax = 0;
	for (int i = 1; i < mon_instance.n + 1; i++)
	{
		if (mon_vecteur.cout < mon_vecteur.ST[i][mon_instance.m] + mon_instance.p[i][mon_instance.m])
		{
			mon_vecteur.cout = mon_vecteur.ST[i][mon_instance.m] + mon_instance.p[i][mon_instance.m];
			nmax = i;
		}
	}
	a = nmax * mon_instance.m;
	mon_vecteur.PERE[mon_instance.n * mon_instance.m + 1] = a;
	//mon_vecteur.cout = mon_vecteur.cout + mon_instance.p[mp[mc].pc][mp[mc].rc];
}
void Generer_vecteur_aleatoire(T_INSTANCE& mon_instance, T_VECTEUR& mon_vecteur) {
	int taille = mon_instance.n * mon_instance.m + 1;
	int* vecteur = new int[taille];

	//int vecteur[51];
	int random;
	vector <int> a;
	int b[200];
	for (int i = 1; i < taille; i++)
	{
		a.push_back(i);
	}
	for (int i = 1; i < taille; i++)
	{
		random = rand() % a.size() + 1;
		//cout << random << " "<<endl;
		b[i] = a[random - 1];
		a.erase(a.begin() + random - 1);
	}
	for (int i = 1; i < taille; i++)
	{
		vecteur[i] = b[i] % mon_instance.n + 1;
	}
	for (int i = 1; i < taille; i++)
	{
		mon_vecteur.V[i] = vecteur[i];
		//cout << mon_vecteur.V[i]<<"	";
	}

}


void Rechercher_locale(T_INSTANCE& mon_instance, T_VECTEUR& mon_vecteur, int nb_itera) {
	int i, j, nb;
	T_VECTEUR vecteur1;
	nb = 0;
	j = -1;
	int o1, n1, o2, n2; 
	i = a;
	j = mon_vecteur.PERE[i];
	while (j != 0 && nb < nb_itera)
	{
		

		for (int i = 0; i < 200; i++)
		{
			vecteur1.V[i] = mon_vecteur.V[i];
		}

		int piece_op1 = 0;
		int piece_op2 = 0;
		if (i % mon_instance.m == 0)
		{
			n1 = (i / mon_instance.m);
			o1 = mon_instance.m;
		}
		else
		{
			n1 = (i / mon_instance.m) + 1;
			o1 = i % mon_instance.m;
		}
		if (j % mon_instance.m == 0)
		{
			n2 = (j / mon_instance.m);
			o2 = mon_instance.m;
		}
		else
		{
			n2 = (j / mon_instance.m) + 1;
			o2 = j % (mon_instance.m);
		}
		
			for (int k = 1; k < mon_instance.m * mon_instance.n + 1; k++)
			{
				if (mon_vecteur.V[k] == n1)
				{
					piece_op1++;
					if (piece_op1 == o1)
					{
						vecteur1.V[k] = n2;
						break;
					}
				}
			}
			for (int k = 0; k < mon_instance.m * mon_instance.n + 1; k++)
			{
				if (mon_vecteur.V[k] == n2)
				{
					piece_op2++;
					if (piece_op2 == o2)
					{
						vecteur1.V[k] = n1;
						break;
					}
				}
			}
			Evaluer(mon_instance, vecteur1);
			if (vecteur1.cout < mon_vecteur.cout)
			{
				mon_vecteur = vecteur1;
				Evaluer(mon_instance, mon_vecteur);
				i = a;
				j = mon_vecteur.PERE[i];
				//cout << mon_vecteur.cout << endl;
			}
			else
			{
				i = j;
				j = mon_vecteur.PERE[j];
			}
		nb++;
	}
}
int Calculer_signature(T_VECTEUR& solution, int nb) {
	int s, r;
	r = 0;
	for (int i = 1; i < nb - 1; i++)
	{
		s = solution.V[i] * solution.V[i] * solution.V[i + 1];
		r += s;
	}
	return r / 100;
}
void Generer_vecteur_voisin(T_VECTEUR& solution, T_VECTEUR& voisin, int nb) {
	int a, b, c;
	a = b = c = 0;
	voisin = solution;
	while (a == b || voisin.V[a] == voisin.V[b])
	{
		a = (rand() % nb) + 1;
		b = (rand() % nb) + 1;
	}
	c = voisin.V[a];
	voisin.V[a] = voisin.V[b];
	voisin.V[b] = c;
};
void Rechercher_global(T_INSTANCE& mon_instance, T_VECTEUR& mon_vecteur, int nb_iteration) {

	T_VECTEUR solution, voisin;
	//Generer_vecteur_aleatoire(mon_instance, voisin);
	int sign_tab[10001] = {0};
	int nbElementVecteur = mon_instance.m * mon_instance.n;

	for (int i = 1; i < nb_iteration + 1; i++)
	{
		Generer_vecteur_aleatoire(mon_instance, solution);
		solution = mon_vecteur;
		int random = 0;
		int sign = Calculer_signature(solution, nbElementVecteur);
		if (sign_tab[i]!=sign)
		{
			while (random < 5) {
				Generer_vecteur_voisin(solution, voisin, nbElementVecteur);
				random++;
				Evaluer(mon_instance, voisin);
				Rechercher_locale(mon_instance, voisin, 100);

				if (voisin.cout < mon_vecteur.cout)
				{
					mon_vecteur = voisin;
				}

		}

			}
		sign_tab[i] = sign;
	}
	cout << mon_vecteur.cout << endl;
}
int main() {
	T_INSTANCE mon_instance;
	T_VECTEUR mon_vecteur;
	srand((unsigned)time(0));
	int S_cout = 10000;
	Lecteur("la10.txt", mon_instance);
	
	Generer_vecteur_aleatoire(mon_instance, mon_vecteur);
	Evaluer(mon_instance, mon_vecteur);
	Rechercher_global(mon_instance, mon_vecteur, 1000);
	
	return 0;
}