X
Parfait. Fais ça en 2 parties: nettoyer l’historique, puis empêcher les prochains fichiers.

1. Supprimer tous les vgcore.* des anciens commits

Attention: ça réécrit l’historique. Il faudra faire un push ls 
 ensuite.

Installe git-filter-repo (si pas déjà installé), puis lance dans ton repo:

    git filter-repo --path-glob 'vgcore.*' --invert-paths

Ensuite pousse l’historique réécrit:

    git push origin --force --all
    git push origin --force --tags

Si tu n’as qu’une branche principale, tu peux faire:

    git push origin --force main

2. Empêcher de les push à l’avenir

Ajoute la règle d’ignore:

    echo 'vgcore.*' >> .gitignore

Puis commit cette règle:

    git add .gitignore
    git commit -m "ignore core dump files (vgcore.*)"
    git push

Important
1. Si tu travailles en équipe, préviens avant le force push (ils devront resync leur branche).
2. Si un vgcore est déjà tracké dans le commit courant, retire-le de l’index:
   
    git rm --cached vgcore.*
    git commit -m "remove vgcore files from tracking"
    git push

Si tu veux, je peux te donner la version “sécurisée” avec branche backup avant le nettoyage (recommandé).








Tu es déjà proche. Pour faire une vraie tokenisation avec guillemets, le plus simple est de passer à une logique en **machine d’états**.

1. Définis les règles de tokenisation
- Séparateurs hors guillemets: espace, tab.
- Opérateurs spéciaux: `|`, `<`, `>`, `<<`, `>>`.
- Guillemets simples `'...'`: tout est littéral.
- Guillemets doubles `"..."`: on garde le bloc ensemble (et plus tard tu feras l’expansion `$VAR` si besoin).
- Décide si tu gardes ou enlèves les guillemets dans `value` (souvent on les enlève).

2. Crée un état de lecture
- `NORMAL`
- `IN_SQUOTE`
- `IN_DQUOTE`

3. Parcours la ligne caractère par caractère
- En `NORMAL`:
  - espace/tab: termine le token courant (si non vide).
  - `'` -> passe en `IN_SQUOTE`.
  - `"` -> passe en `IN_DQUOTE`.
  - `|`, `<`, `>` -> termine token courant puis crée un token opérateur (`<`/`>` doivent tester `<<`/`>>`).
  - sinon: ajoute au buffer courant.
- En `IN_SQUOTE`:
  - si `'` -> retour `NORMAL`
  - sinon ajoute au buffer.
- En `IN_DQUOTE`:
  - si `"` -> retour `NORMAL`
  - sinon ajoute au buffer.

4. À la fin de la ligne
- Si état != `NORMAL`: erreur “quote non fermée”.
- Sinon flush du dernier buffer en token `WORD`.

5. Sépare bien “scanner” et “création de noeud”
- Un scanner produit `(type, start, len)` ou une string temporaire.
- Une fonction dédiée crée le `t_token` et l’ajoute à la liste.
- Ça t’évitera les bugs de boucle/indices.

6. Corrige les points faibles de ton code actuel
- Après fermeture de quote, ton index peut mal avancer selon les cas: risque de retraiter le même caractère.
- Tu ne découpes pas encore les opérateurs spéciaux quand ils sont collés à un mot (`echo|cat`, `a>>b`).
- Ta boucle actuelle découpe surtout par espaces, donc elle ne respecte pas encore complètement les règles shell.

7. Valide avec des cas tests précis
- `echo bonjour`
- `echo "hello world"`
- `echo 'hello   world'`
- `cat <in | grep "abc def" >>out`
- `echo "a'b'c"`
- `echo 'a"b"c'`
- `echo "unterminated` -> doit renvoyer erreur quote non fermée

8. Ensuite seulement: expansion et parsing
- D’abord tokenisation propre.
- Puis expansion (`$VAR`) selon contexte (pas dans `'...'`, oui dans `"..."`).
- Puis parser (pipes/redirections/commands).

Si tu veux, je peux te proposer la version exacte de `parse_line` refactorisée (en C, compatible avec ta structure `t_token`) en gardant ton style actuel.