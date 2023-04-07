# Projet_Ratnasinghe_Rotsztein

Voici le repository associé à notre projet d'IA et systèmes embarqués.

## Contenu et outils

- Un notebook contenant notre modèle entraîné ainsi que nos réponses aux questions et divers commentaires
- Un dossier contenant le modèle entraîné, quantifié et sauvegardé dans un fichier ".h" (d_qat_model_data.h), ainsi que notre code Arduino à intégrer au sein du Tiny ML Kit
- En dehors de ce répertoire, nous avons également un TinyML Kit, contenant une Arduino Nano 33 BLE Sense, une Arduino Shield, et un module caméra Arducam OV7675.

## Téléversement du code Arduino

Il est important de noter que notre code Arduino se compile et est opérationnel. Cependant, nous avons rencontré un problème avec notre kit et l'arduino n'est plus reconnu sur les ports de nos PC. Sur chacun de nos PC, nous avons désinstallé et réinstallé les ports, forcé un Reset manuel (via un appui continu sur le bouton Reset de l'Arduino au moment de brancher la carte, jusqu'à ce qu'une LED orange, celle du Blink code de base, apparaisse, mais impossible de téléverser notre code sur la carte).

Nous avons donc produit un code qui se compile, ne retoure aucun problème et qui est prêt à être déployé, mais sans avoir l'occasion de le tester réellement.

Compilation du code :

![image](https://user-images.githubusercontent.com/66918934/230586770-3ff4fc24-0de6-4319-aec9-3b19601b3483.png)

## Poids du modèle

Comme expliqué dans le notebook, notre modèle est également restreint de par son poids. Initialement, nous avions prévu un modèle avec un peu plus de couches et plus de filtres, mais nous avons dû le réduire pour ne pas déborder sur l'espace mémoire. Avec le modèle actuel, nous occupons 63% de l'espace mémoire d'une Arduino Nano 33 BLE.
