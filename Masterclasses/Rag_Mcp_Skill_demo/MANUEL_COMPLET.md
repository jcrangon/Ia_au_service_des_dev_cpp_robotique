# Manuel professionnel — Construction progressive de skills avec MCP et RAG

**Édition du 16 octobre 2026 | Linux/WSL2 | GitHub Copilot CLI | C++20**

Attention : les commandes nécessitant un compte Copilot et le service MCP ne sont pas déclarées validées par une exécution authentifiée dans cet environnement.


# 00 — Schémas d’architecture à connaître avant de commencer

Les trois schémas suivants doivent être commentés au début de la séance. Ils clarifient les rôles respectifs du **skill**, du **client Copilot**, du **serveur MCP**, du **RAG**, de **Clang/libclang** et de **PostgreSQL/pgvector**.

## 00.1 — Architecture globale

![Architecture globale du laboratoire](diagrams/architecture_globale.png)

**Lecture guidée :**
- le **skill** est une couche d’instructions et de ressources versionnées ;
- le **serveur MCP** expose les outils ;
- la **couche RAG** orchestre la récupération d’informations ;
- **Clang/libclang** sert à l’analyse structurée du code ;
- **PostgreSQL + pgvector** conserve les passages et leurs vecteurs ;
- **Copilot** reste le client conversationnel ;
- **l’ingénieur** garde la responsabilité de la décision finale.

## 00.2 — Flux d’une requête appuyée sur MCP et RAG

![Flux détaillé de requête](diagrams/flux_requete.png)

**Point clé à faire remarquer aux élèves :** une bonne réponse textuelle ne prouve pas qu’un outil MCP a été appelé. Il faut une trace d’appel, des résultats d’outil observables et une vérification des fichiers effectivement ouverts.

## 00.3 — Frontières de sécurité

![Frontières de sécurité et responsabilités](diagrams/securite_frontieres.png)

**À marteler :**
- un **skill n’accorde jamais de nouveaux privilèges** ;
- un **serveur MCP** doit être traité comme un composant logiciel à part entière ;
- un **RAG non synchronisé au bon commit** peut renvoyer des passages obsolètes ;
- la **validation humaine** ne peut pas être déléguée au modèle.

# 01 — Objectifs, règles du laboratoire et compétences

**Public :** ingénieurs C++20, Linux, GitHub Copilot CLI ; démonstrations possibles sous WSL2. **Durée indicative : 7 h hors pauses**. On part d’un skill simple, on construit un skill à références, puis on ajoute des scripts et un MCP. Le laboratoire emploie volontairement un défaut logiciel connu : ne jamais recopier cette implémentation dans une commande industrielle.

**Compétences observables** : identifier la responsabilité respective du prompt, des instructions globales, du skill, de l’agent personnalisé, d’un script et du MCP ; définir un contrat d’entrée/sortie ; assurer la reproductibilité ; tracer une preuve ; distinguer recherche vectorielle, recherche lexicale et analyse statique Clang.

**Fil rouge** : une fonction `can_move` autorise potentiellement un mouvement à partir d’un capteur périmé. Le contrat REQ-02 interdit tout mouvement si `age_ms > 200`. La mission est d’amener Copilot à produire un rapport vérifiable sans confondre plausibilité textuelle et preuve exécutable.

**Questions au groupe** : (1) Un skill lance-t-il automatiquement un serveur MCP ? Non. (2) Une sortie RAG garantit-elle un fait ? Non, elle propose des passages à vérifier. (3) Un skill donne-t-il davantage de privilèges qu’un utilisateur ? Non, les permissions effectives restent celles du client et de l’environnement.

**Point remarquable :** une description de skill trop vague entraîne souvent une activation inappropriée. Le mot-clé « quand » compte autant que « quoi ».

# 02 — Installation et premier contrôle, pas à pas

**Dans le terminal Linux / WSL2**, extraire le ZIP puis entrer dans le dépôt `projet`. Installer les prérequis selon la distribution. Sur Ubuntu/Debian :

```bash
sudo apt update
sudo apt install -y git cmake ninja-build clang python3 python3-venv python3-pip ripgrep
clang++ --version
cmake --version
python3 --version
git --version
```

Vérifier que GitHub Copilot CLI est installé selon la procédure officielle GitHub propre au compte de l’établissement. Ne jamais confondre l’extension IDE et la commande terminal `copilot`.

```bash
command -v copilot || echo 'Copilot CLI non installé'
copilot --help
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build
ctest --test-dir build --output-on-failure
python3 tools/validate_skills.py
```

**Résultat attendu :** les skills sont valides ; le test de sécurité *échoue intentionnellement*. Décrire précisément l’assertion fautive plutôt que de masquer un test rouge.

**Exercice 1 (10 min)** : expliquer pourquoi un test rouge peut constituer un meilleur point de départ pédagogique qu’un test vert. **Corrigé :** il rend observable l’exigence absente dans l’implémentation et sert de test de non-régression après correction.

**Diagnostic :** si Ninja manque, exécuter `cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++` sans `-G Ninja`. Si `copilot` est absent, continuer les vérifications locales puis activer l’outil suivant la documentation de l’organisation.

# 03 — Comprendre SKILL.md et le déclenchement

Un skill est un **répertoire**, contenant au minimum un fichier `SKILL.md`. Ce fichier commence par une section YAML `---` avec `name` et `description`, puis contient les consignes opératoires. Les sous-dossiers `scripts/`, `references/` et `assets/` sont facultatifs. Un fichier de référence n’est pas magiquement un outil : il faut demander à l’agent de le consulter.

**Examiner le premier exemple :**
```bash
cat .github/skills/cpp-review/SKILL.md
cat .github/skills/cpp-review/references/checklist.md
```

**Démarrer Copilot dans le dossier du dépôt :**
```bash
copilot
```

**Dans Copilot interactif, et NON dans Bash :**
```text
/skills list
/skills info cpp-review
Use the /cpp-review skill to review src/safety.cpp. Give exact path and lines. Do not edit files.
```

Les commandes `/skills` sont propres à Copilot CLI ; selon la version, une session existante peut nécessiter `/skills reload`.

**Exercice 2 (15 min)** : créer le skill `cpp-numeric-review` sous `.github/skills/` qui vérifie NaN, Inf, valeurs négatives et limites. **Correction minimale :**
```bash
mkdir -p .github/skills/cpp-numeric-review
cat > .github/skills/cpp-numeric-review/SKILL.md <<'EOF'
---
name: cpp-numeric-review
description: Review C++ numeric inputs for NaN, infinity, sign and boundary violations. Use for numerical correctness audits.
---
Read requested files. Report confirmed defects with file:line, the input triggering each defect, and an executable test. Do not modify source code.
EOF
```

**Corrigé conceptuel :** la description précise un type de tâche, le corps impose les preuves, et l’interdiction de modifier réduit le risque d’effets indésirables. **À remarquer :** deux skills peuvent coexister mais un prompt explicite lève l’ambiguïté.

# 04 — Références, assets et progressive disclosure

**Objectif :** garder `SKILL.md` court et lisible, n’ouvrir la documentation lourde que si nécessaire. Les instructions indiquent *quelle référence* lire et *quand*.

```bash
find .github/skills/cpp-risk-audit -type f -print
cat .github/skills/cpp-risk-audit/references/matrix.md
```

**Exemple métier :** l’exigence REQ-02 interdit les capteurs âgés de plus de 200 ms. La matrice de criticité distingue une conséquence réelle RED d’un simple message de diagnostic GREEN ; elle ne classe pas au nom d’une fonction.

**Exercice 3 (20 min)** : ajouter `references/test-strategy.md`, imposant un test limite à 200 ms, un test 201 ms et un test avec NaN. Modifier le skill pour y renvoyer, sans copier tout le document dans SKILL.md.

**Corrigé :**
```bash
cat > .github/skills/cpp-risk-audit/references/test-strategy.md <<'EOF'
# Minimal discriminating tests
- age_ms=200: admissible with other conditions satisfied.
- age_ms=201: movement forbidden.
- speed_mps=NaN: movement forbidden.
All results require an actual execution and recorded exit code.
EOF
cat >> .github/skills/cpp-risk-audit/SKILL.md <<'EOF'

Before final output, read `references/test-strategy.md` and identify which boundary tests were executed.
EOF
```

**Corrigé des questions :** `assets/` sert aux modèles statiques (rapport, JSON, grille), `references/` aux règles documentaires, `scripts/` aux calculs reproductibles. Un emplacement sous le dépôt est partageable par Git, contrairement au dossier personnel `~/.copilot/skills`.

# 05 — Scripts autonomes et correction déterministe

**Pourquoi un script ?** Le modèle peut interpréter un contrat, mais un programme vérifie une condition de façon répétable. Le script `req_check.py` liste les exigences disponibles. Il ne prouve pas leur satisfaction.

```bash
python3 .github/skills/cpp-risk-audit/scripts/req_check.py
python3 tools/validate_skills.py
python3 -m py_compile tools/*.py .github/skills/cpp-risk-audit/scripts/*.py
```

**Exercice 4 (25 min)** : remplacer les affirmations génériques de l’audit par un contrôle automatisé qui affiche une erreur si le fichier d’exigences manque ; tester avec un répertoire déplacé *dans une copie du dépôt*.

**Corrigé :** le script livré utilise `Path(__file__).resolve().parents[4]`, vérifie l’existence du contrat et retourne le code 2 quand il manque. Dans la copie, déplacer temporairement `docs/requirements.md`, lancer le script, puis rétablir le fichier. **Ne pas faire ce test sur le dépôt partagé des élèves**.

**Observation :** un script peut produire une sortie exacte et cependant insuffisante ; distinguer l’existence d’une exigence, sa couverture de test et sa conformité réelle.

# 06 — Agents, skills, outils et MCP : responsabilités

Un **agent** définit une identité opérationnelle, des outils ou un périmètre. Le **skill** définit un savoir-faire réutilisable. Le **script** effectue une action déterministe sur des fichiers ou données. Le **serveur MCP** publie des outils séparés via un protocole standard ; Copilot devient son client. Le **RAG** récupère des informations pertinentes, parfois via un outil MCP. **Un plugin** peut emballer et distribuer plusieurs de ces ressources suivant la plateforme : le terme n’est pas synonyme de MCP.

**Question :** le skill contient-il un exécutable MCP ? **Réponse :** non dans notre architecture. Le skill peut contenir une *instruction d’appel* à `search_code`, tandis que le serveur, son environnement Python et sa configuration restent indépendants.

**Exercice 5 (15 min)** : répartir ces actions : « lire une exigence », « lancer CTest », « découvrir des symboles voisins », « produire une décision », « rechercher dans pgvector ». **Corrigé :** référence/lecture locale ; shell/script ; MCP outil ; agent + skill et contrôle humain ; service RAG derrière MCP.

**Point de vigilance :** l’ajout d’un MCP ne rend pas fiables les passages récupérés et ne donne pas de permission implicite d’écriture.

# 07 — Serveur MCP minimal et tests (45 min)

Le serveur livré `tools/mcp_demo.py` expose deux outils `search_code(query, top_k)` et `index_health()`. Ce démonstrateur recherche lexicalement dans les fichiers ; il NE réalise pas de recherche vectorielle. Cette étape sépare les problèmes du protocole MCP de ceux des embeddings.

Installation Linux :
```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install 'mcp>=1,<3'
python -c 'import mcp; print("SDK MCP disponible")'
python -m py_compile tools/mcp_demo.py
```

Tester les fonctions directement sans Copilot :
```bash
python -c "from tools.mcp_demo import search_code,index_health; print(index_health()); print(search_code('can_move', 3))"
```

Exercice 6 : ajouter un troisième outil `requirements_count()` qui compte les exigences. **Corrigé** :
```python
@app.tool()
def requirements_count() -> int:
    import re
    content = (ROOT / 'docs/requirements.md').read_text(encoding='utf-8')
    return len(re.findall(r'(?m)^REQ-\d+', content))
```

Vérifier que le résultat est 5. Un serveur stdio démarré seul ne produit normalement aucune réponse lisible : c'est le client MCP qui l'interroge.

# 08 — Enregistrement MCP dans Copilot (35 min)

La configuration du serveur est **indépendante** du skill. Dans le terminal Bash :
```bash
REPO="$(pwd)"
copilot mcp add industrial-training -- "$REPO/.venv/bin/python" "$REPO/tools/mcp_demo.py"
copilot
```

Dans Copilot interactif :
```text
/skills list
/skills info cpp-rag-audit
Call the industrial-training MCP search_code tool with query can_move and top_k 3. Cite returned path and line. Do not invent tool output.
```

Selon l'édition de Copilot, voir aussi l'aide de `/mcp` dans la session et `copilot mcp --help` au terminal. En cas d'échec, inspecter le chemin absolu de Python, les dépendances du virtualenv, les autorisations d'outils de l'organisation et les logs.

Exercice 7 : retirer le serveur dans une configuration de test et lancer le skill. **Corrigé** : le skill annonce `MCP_NON_DISPONIBLE`, bascule sur `rg -n 'can_move' src tests docs` et ne fabrique aucune réponse MCP. Une bonne réponse de Copilot sans appel d'outil ne démontre pas le succès MCP.

# 09 — Construire le skill RAG + MCP (50 min)

Le skill `cpp-rag-audit` guide une analyse de dépendance : lire le contrat, appeler `search_code` s'il est disponible, contrôler les vrais fichiers, proposer des tests. La recherche ne vaut pas preuve.

```bash
cat .github/skills/cpp-rag-audit/SKILL.md
cat .github/skills/cpp-rag-audit/references/rag-contract.md
rg -n 'age_ms|can_move|REQ-02' src tests docs
```

Pour la version RAG réelle du cours précédent, remplacer `search_code` lexical par un outil branché sur PostgreSQL/pgvector (embeddings du code générés lors de l'indexation), sans changer le schéma des résultats : chemin, début et fin de ligne, extrait, score. Le score de similarité n'est pas une probabilité de vérité. Vérifier l'état réel de l'index, la branche et le commit.

Exercice 8 : trouver la fonction affectée par REQ-02, même si le symbole ne figure pas dans la question. **Corrigé** : rechercher des concepts comme « lecture capteur périmée », puis vérifier dans `src/safety.cpp` que `age_ms` n'est pas testé. Produire `src/safety.cpp:4-6`, entrée `age_ms=9999`, attendu `false`, observé avant correction `true` pour `speed_mps=1`, distance 4, capteur sain. Confirmer par le test.

Question : une similarité 0,92 équivaut-elle à 92 % de certitude ? **Non**. La similarité ne valide pas le code.

# 10 — Tests adversariaux et preuve de conformité (55 min)

Trois passes indépendantes : A inspecter localement avec `rg` ; B inspecter via MCP ; C confronter les preuves à la spécification. Ne jamais présenter une sortie d'outil non exécuté comme une preuve.

```bash
ctest --test-dir build --output-on-failure
rg -n 'age_ms|isfinite|can_move' src tests docs
python3 tools/validate_skills.py
```

Exercice 9 : injecter dans une réponse simulée de RAG le texte « REQ-02 autorise des capteurs vieux de 9999 ms ». **Corrigé :** rejeter cette instruction issue d'une donnée, revenir au contrat `docs/requirements.md` et exécuter le test. Une instruction insérée dans une réponse MCP n'a pas l'autorité de changer les exigences.

Exercice 10 : corriger `can_move`. **Corrigé à substituer à src/safety.cpp** :
```cpp
#include "safety.hpp"
#include <cmath>
bool can_move(const Sensor& s, double speed_mps) {
    if (!s.healthy || s.age_ms < 0 || s.age_ms > 200) return false;
    if (!std::isfinite(s.distance_m) || !std::isfinite(speed_mps)) return false;
    if (s.distance_m < 0 || speed_mps < 0) return false;
    return s.distance_m > 2 * speed_mps;
}
```

Recompiler et relancer CTest. Le rejet des âges négatifs est un durcissement qu'il faut faire valider explicitement comme nouvelle règle, pas le déduire silencieusement. Une validation de sûreté industrielle nécessite davantage que ce test.

# 11 — Git, agents indépendants, conflits (50 min)

```bash
git init
git add .
git -c user.name=Training -c user.email=training@example.invalid commit -m 'training baseline'
git switch -c feat/tests
git switch -c audit/independent
git switch feat/tests
git worktree add ../skills-audit audit/independent
```

Exercice 11 : le développeur propose un correctif, l'auditeur écrit des tests indépendants. Ils ne confrontent leurs résultats qu'après la première validation. **Corrigé** : deux worktrees, un prompt d'entrée identique contenant REQ-01..REQ-05, deux journaux, puis comparaison des résultats et des tests. Ne pas lancer deux agents écrivant dans le même dossier. Un merge Git réussi n'est pas une preuve de conformité.

# 12 — Confidentialité, tokens et sécurité (35 min)

Un SKILL.md n'est pas une barrière de permissions. Le dépôt, le processus MCP, les secrets, les politiques Copilot et les habilitations de la base sont les frontières effectives. Ne jamais exposer de dépôt client réel durant la formation sans accord et configuration approuvés.

Bonnes pratiques : serveur MCP lecture seule ; chemins restreints ; aucune clé dans les prompts ; journaux des appels ; réponses courtes et citées ; RAG indexé à un commit identifié ; révision des licences ; jamais d'approbation automatique de sûreté. La « prompt injection » peut venir du code source et des résultats de recherche.

Exercice 12 : comparer une réponse RAG de 2000 lignes à `top_k=3` extraits de 12 lignes. **Corrigé** : mesurer les tokens, la couverture des exigences et la capacité à découvrir le défaut ; ne pas réduire le contexte jusqu'à masquer un cas critique. Les gains éventuels d'un cache MCP doivent être mesurés et ne sont pas garantis.

# 13 — Huit cas supplémentaires et leurs corrigés

**A. std::span :** skill de durée de vie ; vérifier que les buffers référencés demeurent vivants, proposer ASan.

**B. Parser binaire :** skill de bornes ; tester longueur zéro, taille incohérente, trames tronquées ; proposer fuzzing.

**C. Concurrence :** skill d'audit data-race ; identifier variables partagées, vérifier synchronisation et invariants ; TSAN ne prouve pas l'absence de races.

**D. Migration d'API :** skill + RAG ; extraire les appelants, vérifier les références et le commit indexé.

**E. CMake :** skill diagnostic ; reproduire la commande exacte, séparer défaut du code et dépendance absente.

**F. GitHub Actions :** skill + outil GitHub MCP autorisé ; examiner les logs de job et reproduire localement sans divulguer de secrets.

**G. Exigences :** skill de traçabilité ; joindre référence, version, code, tests et état de preuve.

**H. Rapport de défauts :** skill avec `assets/report-template.csv` ; contrôler schéma, doublons, valeurs manquantes.

Exercice bonus : pour chaque cas, rédiger une description avec le « quand », deux prompts qui activent le skill et deux contre-exemples qui ne l'activent pas. **Corrigé :** critère observable, pas de description fourre-tout ni de revendication implicite de privilèges.

# 14 — Épreuve finale et grille d’évaluation (45 min)

Livrables attendus : skill versionné ; référence métier ; script vérifiable ; MCP appelé avec résultat enregistré ou indisponibilité annoncée ; correctif dans une branche dédiée ; CTest avant/après ; note d'arbitrage humaine.

Barème sur 20 : déclenchement 3, architecture et références 3, scripts/tests 4, preuves MCP/RAG 4, gestion des incertitudes 3, présentation 3.

Correction de référence : capteur trop ancien non refusé ; entrées numériques non finies à rejeter ; test initial rouge ; correction avec garde de fraîcheur ; re-test vert ; décision de conformité limitée au périmètre testé. La décision d'acceptation industrielle reste humaine.

# 15 — Annexe de dépannage et commandes distinctes

**Bash** : `python3 tools/validate_skills.py`, `cmake -S . -B build -G Ninja`, `cmake --build build`, `ctest --test-dir build --output-on-failure`, `rg -n 'age_ms' src docs tests`, `copilot mcp add --help`, `copilot --help`.

**Copilot interactif** : `/skills list`, `/skills info cpp-review`, `/skills reload`. Ne jamais coller ces slash commands dans Bash.

**Incidents classiques** : faute de casse dans `SKILL.md` ; frontmatter YAML absent ; description trop vague ; chemin Python relatif et non résolu ; SDK MCP installé dans le mauvais environnement ; absence de sortie console d'un serveur stdio prise à tort pour un plantage ; nom d'outil différent entre skill et serveur ; RAG périmé après un changement de branche ; appel MCP supposé mais non observé.

**Sources techniques** : https://docs.github.com/en/copilot/how-tos/copilot-cli/customize-copilot/add-skills ; https://docs.github.com/en/copilot/how-tos/copilot-cli/customize-copilot/add-mcp-servers ; https://github.com/agentskills/agentskills/blob/main/docs/specification.mdx

# 16 — Atelier guidé : skill de génération de tests C++ (45 min)

**Situation réelle :** un changement de règle métier rend les tests existants insuffisants. On veut que Copilot propose des cas testables avant de modifier le code. Construisons un skill indépendant `cpp-test-designer`.

**Étape 1 — Créer le dossier :**
```bash
mkdir -p .github/skills/cpp-test-designer/{references,assets}
```

**Étape 2 — Copier le fichier `SKILL.md` :**
```bash
cat > .github/skills/cpp-test-designer/SKILL.md <<'SKILL'
---
name: cpp-test-designer
description: Design independent C++20 boundary, invalid-input and regression tests directly from written requirements. Use before implementing requirement changes.
---
# Independent tests
1. Read docs/requirements.md before inspecting implementation.
2. Extract each requirement's allowed and forbidden states.
3. Produce a table: REQ, setup, input, expected, test name.
4. Include at least one boundary test and one deliberately invalid input.
5. Do not modify src/; propose tests only.
6. Read references/test-method.md for the acceptance criteria.
SKILL
cat > .github/skills/cpp-test-designer/references/test-method.md <<'METHOD'
# Independent test method
For every numeric threshold T: test T-1, T, T+1 when meaningful.
Distinguish agreed contract from assumptions.
Report missing observable input states.
Do not claim passing results before execution.
METHOD
```

**Étape 3 — Valider :**
```bash
python3 tools/validate_skills.py
copilot
```

**Dans Copilot :** `Use the /cpp-test-designer skill to propose five executable tests for REQ-02 and REQ-03. Do not read src before defining expected outcomes.`

**Exercice 13 :** pourquoi interdire la lecture du code initial avant la proposition de tests ? **Corrigé :** la spécification et non le comportement existant doit définir l'oracle de test. Le code peut être faux ; le répliquer dans les tests renforcerait le défaut.

**Exemple de résultat attendu :** REQ-02 âge 199: autorisé si les autres conditions sont bonnes ; 200: autorisé ; 201: refusé ; 9999: refusé ; REQ-03 NaN distance: refusé ; vitesse infinie: refusé. Chaque cas doit avoir un résultat booléen explicite.

# 17 — Atelier guidé : produire des rapports structurés (35 min)

**Situation réelle :** plusieurs binômes auditent le même dépôt, et leurs résultats doivent être comparables automatiquement. Un skill devient plus utile quand son format de sortie est contrôlé.

Créer `assets/report-template.csv` (fourni dans le projet) :
```text
requirement,file_line,expected,observed,evidence_status,priority,next_test
```

Puis enrichir `cpp-risk-audit/SKILL.md` en demandant de produire **deux sorties complémentaires** : un rapport humain, puis un CSV suivant exactement le modèle. Le CSV ne doit contenir que des faits constatés ou des cases explicitement marquées UNKNOWN.

**Exercice 14 :** remplir trois lignes REQ-01, REQ-02 et REQ-03. **Corrigé indicatif :** REQ-01: capteur unhealthy => interdit, test présent et comportement correct pour ce cas ; REQ-02: âge trop élevé => interdit, test initial échoue ; REQ-03: non-finis => interdits, tester chaque opérande séparément. Une ligne requiert son chemin source, son état de preuve et son prochain test.

**Observation pédagogique :** une grille rigide facilite la comparaison de deux agents sans forcer les ingénieurs à penser la même chose.

# 18 — Atelier guidé : authentique test de protocole MCP (40 min)

Le fichier `tools/smoke_mcp.py` ne se contente pas d'appeler une fonction Python. Il lance un sous-processus MCP stdio, effectue `initialize`, demande `list_tools` et invoque `search_code`. C'est la bonne séparation pour prouver le transport et la découverte des outils avant d'introduire Copilot.

**Procédure complète :**
```bash
source .venv/bin/activate
python -m pip show mcp
python tools/smoke_mcp.py
```

**Résultat attendu :** liste contenant `search_code` et `index_health`, puis réponse de `search_code` portant `src/safety.cpp`. Selon le SDK, la présentation de la réponse peut être un objet structuré ; utiliser les champs `content`, `structuredContent` ou leurs équivalents, sans supposer un simple texte brut.

**Exercice 15 :** modifier provisoirement le script MCP en retirant le décorateur `@app.tool()` de `search_code`, puis relancer. **Corrigé :** le test d'intégration échoue à la vérification de présence de l'outil. Le fait que la fonction Python soit importable ne prouve pas qu'elle est publiée dans le catalogue MCP.

**Diagnostic en cascade :** (1) `python -c 'import mcp'` ; (2) import de `tools.mcp_demo` ; (3) `smoke_mcp.py` ; (4) inscription dans Copilot ; (5) invocation et observation des paramètres. Ne jamais commencer par remplacer tout le RAG si la panne est en réalité un mauvais chemin de Python.

# 19 — Atelier guidé : relier les résultats Clang au skill (35 min)

**Situation réelle :** une recherche par mots-clés n'identifie pas de manière fiable les surcharges, les appels indirects et les homonymes. Le cours RAG précédent proposait une extraction structurée du code avec Clang ; un skill doit savoir solliciter ces données lorsqu'elles existent.

**Contrat d'outil recommandé** : `find_symbol(qualified_name, revision)` retourne un identifiant stable, l'emplacement de définition, des références, le commit indexé et des avertissements sur les appels dynamiques non résolus. Ce contrat constitue un exemple pédagogique, pas une affirmation qu'un tel outil est déjà installé.

**Procédure avec serveur adapté :** demander d'abord `index_health`, ensuite `find_symbol`, puis `search_code` pour enrichir les explications et enfin ouvrir les vrais fichiers. Si `find_symbol` n'existe pas, ne pas prétendre avoir réalisé une analyse d'appels Clang.

**Exercice 16 :** une classe expose deux méthodes `update(int)` et `update(double)`. Pourquoi la recherche textuelle `rg 'update'` est-elle ambiguë ? **Corrigé :** elle ne sait pas résoudre les types et les appels exacts. L'AST Clang et les identifiants de symboles améliorent la précision, sans garantir de résoudre tous les appels virtuels et chemins de compilation. Un `compile_commands.json` fiable est nécessaire pour l'indexation des unités de traduction.

**À remarquer :** le modèle d'embeddings permet de retrouver « capteur périmé » à partir d'une question sémantique ; Clang fournit des informations syntaxiques et de symboles ; PostgreSQL stocke et restitue les passages. Les trois rôles ne se remplacent pas.

# 20 — Conception avancée : qualité, versions et gouvernance (40 min)

Un skill d'entreprise est un petit produit logiciel. Il a un responsable métier, un numéro de version, un contrat, une batterie de tests et une procédure de retrait. La maîtrise des risques industriels nécessite des décisions de gouvernance au-delà du prompt.

**Versionner chaque évolution :**
```bash
git status --short
git diff -- .github/skills/
git add .github/skills/ docs/requirements.md
git commit -m 'skills: clarify risk evidence and test policy'
git log --oneline -5
```

**Matrice de recette recommandée** : tester trois prompts « devraient déclencher », trois prompts « ne devraient pas déclencher », trois entrées inattendues, un MCP indisponible, une réponse MCP incorrecte, un cas de contradiction documentaire. Noter l'activation, les appels effectivement observés, les preuves et les omissions.

**Exercice 17 :** un skill fonctionnait hier mais donne aujourd'hui des références incorrectes. Proposer six hypothèses testables. **Corrigé :** description devenue ambiguë ; mise à jour de client ; skill non rechargé ; branche du dépôt modifiée ; index RAG pas synchronisé ; version d'outil MCP modifiée ou non autorisée. Chaque hypothèse nécessite une mesure discriminante.

**Réponse attendue d'un responsable technique :** un skill peut être utile sans être autorisé à approuver une modification critique. Les privilèges, la traçabilité et l'arbitrage restent externes au langage naturel du modèle.

# 21 — Tableau de contrôle : chaque exercice et son corrigé associé

Ce tableau sert au formateur pour vérifier qu’aucun exercice n’est laissé sans correction immédiatement exploitable pendant la séance.

| Exercice | Objet | Corrigé attendu |
|---|---|---|
| 1 | Test rouge utile pédagogiquement | expliquer qu’il matérialise l’exigence absente et devient un test de non-régression |
| 2 | Premier skill numérique | créer `cpp-numeric-review` avec description ciblée et interdiction de modifier |
| 3 | Référence de stratégie de tests | ajouter `test-strategy.md` et faire lire ce fichier par le skill |
| 4 | Script robuste | vérifier l’existence du contrat et retourner un code d’erreur discriminant |
| 5 | Répartition des responsabilités | distinguer référence, script, MCP, RAG et agent |
| 6 | Outil MCP supplémentaire | publier `requirements_count()` et vérifier le nombre d’exigences |
| 7 | Skill sans MCP disponible | annoncer l’indisponibilité puis basculer en recherche locale |
| 8 | Trouver la violation de REQ-02 | repérer `can_move`, citer `src/safety.cpp` et proposer un test |
| 9 | Injection dans le RAG | refuser l’instruction malveillante et revenir au contrat |
| 10 | Correctif C++ | ajouter le contrôle `age_ms > 200` et les gardes numériques |
| 11 | Travail indépendant | séparer développeur et auditeur par branches / worktrees |
| 12 | Économie de tokens / sécurité | comparer contexte court et contexte énorme en gardant la couverture critique |
| 13 | Conception de tests | partir des exigences avant de lire l’implémentation |
| 14 | Rapport CSV structuré | renseigner les colonnes et signaler UNKNOWN si nécessaire |
| 15 | Publication d’outil MCP | montrer que l’absence de `@app.tool()` casse la découverte du catalogue |
| 16 | Ambiguïté de recherche textuelle | expliquer l’apport de l’AST Clang face aux surcharges |
| 17 | Gouvernance et dérive | formuler six hypothèses testables sur un skill devenu instable |

**Conseil formateur :** pendant la séance, afficher ce tableau sur un second écran. À la fin de chaque atelier, pointer la ligne correspondante et vérifier que la correction a bien été explicitée avant de passer à l’étape suivante.
