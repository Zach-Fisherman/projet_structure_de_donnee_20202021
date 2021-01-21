#include <stdlib.h>
#include "p_table_des_matieres.h"

t_entree* nouvelle_entree(char* etiquette, a_entree pere, a_entree gd_frere){
    t_entree* new_entree=malloc(sizeof(t_entree));
    new_entree->l_etiquette = etiquette;
    new_entree->le_premier_fils = TABLE_VIDE;
    new_entree->le_dernier_fils = TABLE_VIDE;
    new_entree->le_frere_precedent = gd_frere;
    new_entree->le_frere_suivant = TABLE_VIDE;
    new_entree->le_pere = pere;

    return new_entree;
}

void afficher_entree(a_entree ceci){
    printf("%p : [",*ceci);
    printf("le_premier_fils = %p, ",ceci->le_premier_fils);
    printf("le_dernier_fils = %p, ",ceci->le_dernier_fils);
    printf("le_frere_precedent = %p, ",ceci->le_frere_precedent);
    printf("le_frere_suivant = %p, ",ceci->le_frere_suivant);
    printf("le_pere = %p]",ceci->le_pere);
    printf("%c\n", ceci->l_etiquette);
}

void afficher_table(t_table ceci, int decalage){
    for(int i=0;i<decalage;i++){
        printf("\t");
    }
    printf("%s\n",ceci->l_etiquette);
    if(ceci->le_premier_fils!=TABLE_VIDE){
        afficher_table(ceci->le_premier_fils,decalage+1);
    }
    if(ceci->le_frere_suivant!=TABLE_VIDE){
        afficher_table(ceci->le_frere_suivant,decalage);
    }
}


void creer_aine(t_table ceci, char* etiquette){
    if(ceci->le_premier_fils==TABLE_VIDE){
        ceci->le_premier_fils = nouvelle_entree(etiquette,ceci,TABLE_VIDE);
        ceci->le_dernier_fils = ceci->le_premier_fils;
    }else{
        t_table pmf = nouvelle_entree(etiquette,ceci,TABLE_VIDE);
        if(ceci->le_premier_fils == ceci->le_dernier_fils){
            pmf->le_frere_suivant = ceci->le_premier_fils;
            ceci->le_premier_fils->le_frere_precedent = pmf;
            ceci->le_premier_fils = pmf;
            /*ceci->le_premier_fils = pmf;
            ceci->le_premier_fils->le_frere_suivant = ceci->le_dernier_fils;
            ceci->le_dernier_fils->le_frere_precedent == ceci->le_premier_fils;*/
        }else{
            pmf->le_frere_suivant = ceci->le_premier_fils;
            ceci->le_premier_fils->le_frere_precedent = pmf;
            ceci->le_premier_fils = pmf;
        }
    }

    if(ceci->le_dernier_fils==TABLE_VIDE){
        a_entree temp = ceci->le_premier_fils;
        while(temp->le_frere_suivant!=TABLE_VIDE){
            temp = temp->le_frere_suivant;
        }
        ceci->le_dernier_fils = temp;

    }
}

void creer_benjamin(a_entree ceci, char* etiquette){
    a_entree tmp;
    a_entree tmp2;
    int first_son = 0;

    if(ceci->le_dernier_fils==TABLE_VIDE && ceci->le_premier_fils!=TABLE_VIDE){
        if(ceci->le_premier_fils!=TABLE_VIDE){
            tmp = ceci->le_premier_fils;
            while(tmp->le_frere_suivant!=TABLE_VIDE){
                tmp = tmp->le_frere_suivant;
            }
            tmp2 = nouvelle_entree(etiquette,ceci,tmp);
        }
    }else if(ceci->le_premier_fils!=TABLE_VIDE){
        tmp = ceci->le_dernier_fils;
        tmp2 = nouvelle_entree(etiquette,ceci,tmp);
    }else{
        first_son =1;
    }

    if(first_son){
    creer_aine(ceci,etiquette);
    }else{
        tmp->le_frere_suivant = tmp2;
        tmp2->le_frere_precedent =tmp;
        ceci->le_dernier_fils=tmp2;
    }
}

void creer_petit_frere(a_entree ceci, char* etiquette){
    a_entree tmp = ceci->le_frere_suivant;
    ceci->le_frere_suivant = nouvelle_entree(etiquette,ceci->le_pere,ceci);
    ceci->le_frere_suivant->le_frere_suivant = tmp;
}


void inserer_grand_frere(t_table* ceci, char* etiquette){
    t_table gra_f = nouvelle_entree(etiquette,(*ceci)->le_pere,(*ceci)->le_frere_precedent);    
    t_table tmp = (*ceci);
    gra_f->le_frere_suivant = tmp;
    tmp->le_frere_precedent = gra_f;
    *ceci = gra_f;
}

int remonter(a_entree ceci){
    if (ceci->le_pere == TABLE_VIDE){
        return 0;
    }else{
        a_entree e_pere = ceci->le_pere;
        a_entree e_oncle = ceci->le_pere->le_frere_suivant;
        a_entree e_grand_pere = ceci->le_pere->le_pere;

        if(ceci->le_frere_precedent == TABLE_VIDE && ceci->le_frere_suivant==TABLE_VIDE){
            e_pere->le_premier_fils = TABLE_VIDE;
        }else if(ceci->le_frere_precedent == TABLE_VIDE){
            e_pere->le_premier_fils = ceci->le_frere_suivant;
            e_pere->le_premier_fils->le_frere_precedent = TABLE_VIDE;
        }else if(ceci->le_frere_suivant==TABLE_VIDE){
            e_pere->le_dernier_fils = ceci->le_frere_precedent;
            e_pere->le_dernier_fils->le_frere_suivant =TABLE_VIDE;
        }else{
            ceci->le_frere_precedent->le_frere_suivant = ceci->le_frere_suivant;
        }
        
        ceci->le_frere_precedent = e_pere;
        e_pere->le_frere_suivant = ceci;
        ceci->le_frere_suivant = e_oncle;
        if( e_oncle!=TABLE_VIDE){
            e_oncle->le_frere_precedent = ceci;
        }
        ceci->le_pere = e_grand_pere;
        return 1;
    }
}    

int descendre(a_entree ceci){
    if(ceci->le_frere_precedent == TABLE_VIDE){
        return 0;
    }else{
        if(ceci->le_frere_precedent->le_premier_fils==TABLE_VIDE){
            ceci->le_frere_precedent->le_dernier_fils= ceci;
        }
        a_entree e_ainee = ceci->le_frere_precedent;
        a_entree e_p_frere = ceci->le_frere_suivant;
        
        ceci->le_frere_precedent = TABLE_VIDE;

        e_ainee->le_frere_suivant = e_p_frere;
        e_p_frere->le_frere_precedent = e_ainee;
        //l'ainee pointe maintenant vers le frere suivant de ceci

        ceci->le_frere_suivant = e_ainee->le_premier_fils;
        e_ainee->le_premier_fils = ceci;
        ceci->le_pere = e_ainee;
        //le premier fil de l'ainée est maintenant ceci*/
        
        /*if(ceci->le_premier_fils->le_frere_suivant != TABLE_VIDE){
            if(ceci->le_premier_fils->le_frere_suivant->le_frere_precedent != TABLE_VIDE){
                e_ainee->le_premier_fils->le_frere_suivant->le_frere_precedent = ceci->le_premier_fils;
            }
        }*/
        //sensée enlever le frère précedent de ceci mais fait une Segmentation fault
        
        return 1;
    }
}

void supprimer_entree(a_entree* ceci){
    a_entree temp = (*ceci)->le_dernier_fils;
    a_entree temp2;

    while(temp !=TABLE_VIDE){
        temp2 = temp;
        temp = temp->le_frere_precedent;
        remonter(temp2);
    }

    if ((*ceci)->le_pere == TABLE_VIDE && (*ceci)->le_frere_precedent == TABLE_VIDE){
        temp = (*ceci)->le_frere_suivant;
        temp->le_frere_precedent = TABLE_VIDE;
        *ceci = temp;
        
    }else{
        temp = (*ceci)->le_frere_precedent;
        *ceci = (*ceci)->le_frere_suivant;
        temp->le_frere_suivant=*ceci;
        (*ceci)->le_frere_precedent=temp;
    }   
}

void detruire_table(t_table* ceci){
    while((*ceci)->le_frere_suivant!=TABLE_VIDE){
        supprimer_entree(ceci);       
    }
    free(*ceci);
}
