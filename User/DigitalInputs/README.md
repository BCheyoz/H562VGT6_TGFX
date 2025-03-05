# Anode Lib v1.0


alim anode -> pas besoin de gérer

retour anode pin 5 connecteur J2 ?

TFL4-1577 : La lecture du fonctionnement de l'anode se fait grâce à la pin 5 de J2.
Suivant la référence de l'anode choisi :

    Ariston : Besoin d'isoler la tension secteur grâce à un optocoupleur
    Magontec : Pas besoin d'isoler la tension secteur, déjà fait sur la carte électronique de l'anode
    
pin PA5 GPIO_Input | DI_Anode

