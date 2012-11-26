  #include <stdio.h>
  #include <string.h>
  #include "gmp.h"
  #include <stdlib.h>
  #define N 1000


  void Transcoder(mpz_t code, char M[]) // méssage => nombre
  {
	  long int size_M, i; // size_M == la taille du message
	  size_M = strlen(M);  // recuperé la taille du message
	  mpz_set_ui(code, 0);  // code=0
	  for(i = size_M - 1; i >= 0; i--)
	  {
		  mpz_mul_ui(code, code, 256);  //256*256 
		  mpz_add_ui(code, code, (unsigned long)M[i]); // M(i) * 256^i
	  }
  }

    void Decoder(char M[], mpz_t code) // nombre => message
  {
	  long int size_M, i;
	  mpz_t exposant, c_mod, var_code; 
	  mpz_init(exposant); // 256^i => exposant = i
	  mpz_init(c_mod); // résultat du reste de la division
	  mpz_init(var_code); // prendra la valeur du code
	  mpz_set(var_code, code);
	  /* Calcul de la longueur de la chaîne */
	  mpz_set_ui(exposant, 1);
	  for(i = 0; i < N; i++)
	    {
	      if(mpz_cmp(var_code, exposant) <= 0)
		    {
		      size_M = i;
		      break;
		    }
	      mpz_mul_ui(exposant, exposant, 256); // calcul de l'exposant 
	    }
	  /* Conversion en chaîne  */
	  for(i = 0; i < size_M; i++)
	    {
	      mpz_mod_ui(c_mod, var_code, 256); // reste de la division
	      mpz_sub(var_code, var_code, c_mod); // soustraction
	      mpz_tdiv_q_ui(var_code, var_code, 256); // division
	      M[i] = mpz_get_ui(c_mod); // affeceter le résultat à la chaine de caractère
	    }
	  M[size_M] = '\0';
	  mpz_clear(exposant);
	  mpz_clear(c_mod);
	  mpz_clear(var_code);
  }

    void Taille_bloc(mpz_t max){
      
      mpz_t var_max; // prendra la valeur n
      mpz_init(var_max);
      mpz_set(var_max,max);
      int k=0; // initialisé nombre de caractère au max qu'on peut chiffrer avec n

      do{
      	mpz_divexact_ui(var_max, var_max, 256); // n/256
	k++; // incrémenté nombre de caractère au max qu'on peut chiffrer avec n

      } while (mpz_sgn(var_max) ==1); // tant que var_max != 0
      
      printf("\n*********************************\t La fonction Taille_bloc\t********************************\n");
      printf("Le nombre de caractère au max qu'on peut chiffrer avec n est : %d \n",k);
      printf("\n********************************************************************************************************\n");
      mpz_clear(var_max);
    }
    void Generer_cle(mpz_t n, mpz_t c, mpz_t d, mpz_t val)
    {
      mpz_t p,q,phi,alea,gcdoc; // alea == le nombre aleatoir pour le calcul du p et q // gcdoc variable pour calculer de c 
      gmp_randstate_t initial; 

      // initialisations
      mpz_init(p);
      mpz_init(q);
      mpz_init(d);
      mpz_init(phi);
      mpz_init(alea);
      mpz_init(gcdoc);
      
      
      gmp_randinit_mt(initial);
      mpz_urandomm(alea,initial,val); //alea = genéré une valeur aléatoir inferieur au message
      
      mpz_nextprime(p,alea); // p = 1er nombre premier superieur a alea
      printf("\np et q ont les valeurs suivantes : \n");
      gmp_printf("\n\t\tla valeur de p est %Zd \n",p);

      mpz_nextprime(q,p); // q = 1er nombre premier superieur a p
      gmp_printf("\t\tla valeur de q est %Zd \n",q);
      
      // calcul de n
      mpz_mul(n,p,q); // n = p*q 
      mpz_sub_ui(p,p,1);
      mpz_sub_ui(q,q,1);
      mpz_mul(phi,p,q); // Ø = (p-1) (q-1)
      
      // calcul de c
      mpz_set_ui(c,1); // initialisé c=1
      do{
	mpz_add_ui(c,c,1); // c>1
	mpz_gcd(gcdoc,c,phi); // calcul le PGCD entre la nouveau c est  Ø

	} while (mpz_cmp_ui(gcdoc,1) != 0); // while pgcd(c,phi)!=1
      
      Taille_bloc(n); // appel a la fonction pour calculer le nombre de caractères max qu'on peut chiffrer
      mpz_invert(d,c,phi); // calcul de la clef secrète
      
      mpz_clear(p);mpz_clear(q); mpz_clear(alea); mpz_clear(gcdoc);mpz_clear(phi);
      
      
    }


  void main(){
	  char message[N],message_dechiffre[N]; // message entrée et le message déchiffré
	  	  
	  mpz_t msg_Nb,clef_publique_n, clef_publique_c, clef_privee_d,m2,Nb_msg; // déclaration des differentes cléfs
	  
	  /* Initialisation des variables GMP */
	  
	  mpz_init(clef_publique_n); mpz_init(clef_publique_c); mpz_init(clef_privee_d);
	  mpz_init(m2);mpz_init(Nb_msg);mpz_init(msg_Nb);
	  
	 
	  printf("\n\t\t******************************\n");
	  printf("\t\t*                            *\n");
	  printf("\t\t*      Cryptographie         *\n");
	  printf("\t\t*                            *\n");
	  printf("\t\t*    Zakaria Bouchakor       *\n");
	  printf("\t\t*                            *\n");
	  printf("\t\t******************************\n");
	  
    
	  /* Entrée au clavier d'une chaîne de caractères */
	  printf("\nEntrez le message à crypter :  ", N);
	  fgets(message, N - 1, stdin);/* Lire le message... */
	  
	  if(message[strlen(message) - 1] == '\n')
		  message[strlen(message) - 1] = '\0'; /*marqué la fin du message avec \0*/
	
	  /* Transcoder le message càd Message => nombre */
	  Transcoder(msg_Nb, message);
	  gmp_printf("\nMessage est de taille %d est devient avec la fonction Transcoder %Zd\n\n", strlen(message), msg_Nb);
	  
	  
	  /* Fabrique des clefs */
	  Generer_cle(clef_publique_n,clef_publique_c,clef_privee_d, msg_Nb);
	  printf("\n\tGenerateur des clefs\n");
	  printf("\n\t\t************************************************************************************\n");
	  gmp_printf("\t\tClef_publique_n : %Zd\n", clef_publique_n);
          printf("\t\t*                                                                                  *\n");
	  gmp_printf("\t\tClef_publique_c : %Zd\n", clef_publique_c);
	  printf("\t\t************************************************************************************\n");
	  gmp_printf("\n\t\t*Clef_privee_d: %Zd\t\t\t\t\t\t\t   \n",clef_privee_d);
	  printf("\t\t************************************************************************************\n");
	 
	  
	    /* Cryptage*/
	    if(mpz_cmp(clef_publique_n,msg_Nb)>0) { // si n est supérieur a la valeur transcoder 
	    mpz_powm(m2,msg_Nb,clef_publique_c,clef_publique_n); // chiffrement M^c mod n
	    printf("\n\n\n\t\t************************************************************************************\n");
	    gmp_printf("\t\t*Cryptage= %Zd \t\t\t\t\t\t\t   ", m2); // afficher le message chiffré
	    printf("\n\t\t************************************************************************************\n");
	
	    /*Décryptage*/
	    mpz_powm(Nb_msg,m2,clef_privee_d,clef_publique_n); // décryptage M^d mod n
	    printf("\n\t\t************************************************************************************\n");
	    gmp_printf("\t\t*Décryptage= %Zd \t\t\t\t\t\t\t\t   ",Nb_msg);
	    printf("\n\t\t************************************************************************************\n");
	    }
	    else // n est superieur a la valeur transcoder 
	    printf("message superieur à la valeur n !");
	
	  /*Decoder le message càd nombre => message*/
	  Decoder(message_dechiffre,Nb_msg);
	  printf("\n Le message initial est : ");
	  gmp_printf("%s\n", message_dechiffre);

	  /* Libération de la mémoire utilisée */
	  mpz_clear(msg_Nb); 
	  mpz_clear(clef_publique_n); mpz_clear(clef_publique_c); mpz_clear(clef_privee_d);mpz_clear(m2);mpz_clear(Nb_msg);
	  
  }
