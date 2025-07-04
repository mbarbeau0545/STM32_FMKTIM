1. TIM_OCMODE_TIMING (Frozen)
Description :
En mode "Frozen" ou timing, la sortie ne change jamais d’état, quel que soit le résultat de la comparaison. Ce mode est utilisé lorsqu’on souhaite utiliser le timer uniquement pour le comptage ou pour générer des événements internes sans modifier l’état de la broche de sortie.

Exemple d’utilisation :
Utiliser ce mode pour déclencher une interruption ou pour mesurer un intervalle de temps sans générer de signal visible sur une sortie (par exemple, pour synchroniser des opérations internes).

2. TIM_OCMODE_ACTIVE
Description :
Lorsque la valeur du compteur atteint la valeur de comparaison, la sortie est forcée à l’état actif (souvent logique haut).

Exemple d’utilisation :
Générer un signal d’activation pour démarrer un actionneur ou allumer une LED à un instant précis.

3. TIM_OCMODE_INACTIVE
Description :
À la correspondance du compteur avec la valeur de comparaison, la sortie est mise à l’état inactif (souvent logique bas).

Exemple d’utilisation :
Désactiver un signal, par exemple pour couper un moteur ou éteindre une LED à un moment défini.

4. TIM_OCMODE_TOGGLE
Description :
Ce mode fait basculer la sortie (de haut à bas ou inversement) à chaque fois qu’une comparaison est effectuée.

Exemple d’utilisation :
Créer une onde carrée ou un signal clignotant pour la synchronisation, par exemple pour générer une fréquence d’horloge.

5. TIM_OCMODE_PWM1
Description :
En mode PWM1, la sortie reste active (généralement haut) tant que le compteur est inférieur à la valeur de comparaison, puis devient inactive. Le rapport cyclique détermine la largeur d’impulsion.

Exemple d’utilisation :
Réguler la vitesse d’un moteur ou la luminosité d’une LED par modulation de la largeur d’impulsion.

6. TIM_OCMODE_PWM2
Description :
Mode inverse de PWM1 : la sortie est inactive tant que le compteur est inférieur à la valeur de comparaison et devient active ensuite.

Exemple d’utilisation :
Dans les applications où la polarité du signal PWM doit être inversée par rapport à PWM1 (par exemple, pour piloter des dispositifs avec une logique inversée).

7. TIM_OCMODE_FORCED_ACTIVE
Description :
Ce mode force la sortie à l’état actif indépendamment du compteur et de la valeur de comparaison.

Exemple d’utilisation :
Imposer immédiatement un état actif sur une sortie, par exemple pour forcer l’activation d’un relais en cas de besoin urgent.

8. TIM_OCMODE_FORCED_INACTIVE
Description :
À l’inverse du mode précédent, la sortie est maintenue en état inactif, quel que soit le comptage.

Exemple d’utilisation :
Forcer l’arrêt d’un signal de sortie, utile pour sécuriser ou réinitialiser rapidement un système.

9. TIM_OCMODE_RETRIGERRABLE_OPM1
Description :
Ce mode génère un pulse unique (One Pulse Mode) qui peut être re-déclenché (retriggerable) si un nouvel événement survient avant la fin du pulse.

Exemple d’utilisation :
Dans des applications de communication ou de synchronisation où un pulse unique doit être généré à chaque déclenchement externe, avec la possibilité de réagir rapidement à de nouveaux événements.

10. TIM_OCMODE_RETRIGERRABLE_OPM2
Description :
Variante du mode One Pulse retriggerable avec une configuration (souvent de polarité ou de durée) légèrement différente de OPM1.

Exemple d’utilisation :
Pour des scénarios nécessitant un comportement spécifique lors du retriggering, comme dans certains protocoles de transmission de données où la forme du pulse doit être ajustée.

11. TIM_OCMODE_COMBINED_PWM1
Description :
Mode combiné utilisé principalement avec des timers avancés pour générer des signaux PWM complémentaires synchronisés, souvent avec une gestion de temps mort (dead time).

Exemple d’utilisation :
Contrôler des ponts en H pour la commande de moteurs ou d’onduleurs, où deux sorties complémentaires avec une synchronisation précise sont requises.

12. TIM_OCMODE_COMBINED_PWM2
Description :
Variante du mode combiné avec une configuration de polarité différente, permettant de générer l’inverse du signal de PWM1 combiné.

Exemple d’utilisation :
Utilisé dans le même contexte que le mode combiné PWM1, mais pour des applications où l’inversion de la logique PWM est nécessaire (par exemple, pour adapter des circuits avec des besoins de polarité spécifiques).

13. TIM_OCMODE_ASSYMETRIC_PWM1
Description :
Ce mode permet de générer des sorties PWM complémentaires avec des rapports cycliques différents (asymétriques), offrant une plus grande flexibilité dans la modulation.

Exemple d’utilisation :
Dans des applications de commande moteur nécessitant une compensation fine entre deux signaux complémentaires pour optimiser le contrôle du couple ou la réponse dynamique.

14. TIM_OCMODE_ASSYMETRIC_PWM2
Description :
Variante du mode asymétrique offrant une configuration de base différente, souvent en simplifiant la logique d’inversion par rapport à PWM1 asymétrique.

Exemple d’utilisation :
Pour des systèmes où l’on doit fournir un signal de sortie avec un rapport cyclique ajusté différemment sur une voie complémentaire par rapport à l’autre, comme dans certaines architectures de commande de puissance.

15. TIM_OCMODE_PULSE_ON_COMPARE
Description :
Ce mode génère un pulse court et précis dès que le compteur atteint la valeur de comparaison, puis le signal se réinitialise. Il est limité aux canaux 3 et 4.

Exemple d’utilisation :
Pour déclencher un événement externe ou démarrer une conversion (ex. ADC) avec une impulsion de durée contrôlée précisément.

16. TIM_OCMODE_DIRECTION_OUTPUT
Description :
Ce mode permet de produire une sortie qui reflète la direction de comptage du timer (montée ou descente). Il est également réservé aux canaux 3 et 4.

Exemple d’utilisation :
Utile pour le diagnostic ou pour fournir un retour d’information sur la direction du comptage, par exemple dans des systèmes où la détection de la phase ascendante/descendante est critique.