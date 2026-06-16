# Guide de correction — FdF (ntassin)

---

## 1. Vue d'ensemble

**FdF** (Fil de Fer) est un visualiseur 3D de cartes de hauteur (heightmap).
Il lit un fichier `.fdf` (grille de valeurs z), applique une **projection isométrique**
et trace chaque arête de la grille avec l'algorithme de **Bresenham**.

Le rendu se fait via la **MiniLibX** (bibliothèque graphique bas-niveau X11).

Deux binaires sont produits :
- `./fdf`        → partie obligatoire (commandes : Échap, fermer la fenêtre)
- `./fdf_bonus`  → partie bonus (rotations, zoom, projections, HUD, générateur procédural)

---

## 2. Compilation

```bash
make          # compile ./fdf
make bonus    # compile ./fdf_bonus
make clean    # supprime les .o
make fclean   # supprime les .o + les binaires + les libs
make re       # fclean + all
```

Flags : `-Wall -Wextra -Werror -Wno-cast-function-type -D BUFFER_SIZE=16384`

La MiniLibX et libft sont recompilées automatiquement si nécessaire.

---

## 3. Format du fichier .fdf

```
0  0  0  0
0  5 10  0
0  5  0  0
0  0  0  0
```

- Chaque **ligne** du fichier = une **rangée** de la grille (axe Y).
- Chaque **nombre** = une **colonne** (axe X), sa valeur = **hauteur z**.
- Les tokens sont séparés par des espaces.
- Couleur optionnelle : `10,0xFF0000` (z=10, couleur rouge).
  - La virgule sépare z de la couleur hexadécimale.
  - Sans couleur : le programme calcule un dégradé bleu→blanc selon z.

Exemple avec couleurs : `maps/test_maps/elem-col.fdf`.

---

## 4. Structures de données

### `t_map` — la grille

```c
typedef struct s_map {
    int **z;      // z[row][col] : hauteur de chaque point
    int **color;  // color[row][col] : couleur (-1 = pas de couleur)
    int width;    // nombre de colonnes
    int height;   // nombre de lignes
    int z_min;    // altitude minimale (pour normalisation)
    int z_max;    // altitude maximale (pour normalisation)
} t_map;
```

### `t_cam` — la caméra (obligatoire)

```c
typedef struct s_cam {
    float zoom;     // facteur de zoom (pixels / unité grille)
    int   x_off;    // décalage horizontal de la projection (pixels)
    int   y_off;    // décalage vertical de la projection (pixels)
    float z_scale;  // facteur d'exagération de la hauteur
} t_cam;
```

### `t_cam` — la caméra (bonus, enrichie)

```c
typedef struct s_cam {
    float zoom;
    int   x_off;
    int   y_off;
    float z_scale;
    float rot_x;   // angle de rotation autour de l'axe X (radians)
    float rot_y;   // angle de rotation autour de l'axe Y (radians)
    float rot_z;   // angle de rotation autour de l'axe Z (radians)
    int   proj;    // 0 = isométrique, 1 = parallèle (orthogonale)
} t_cam;
```

### `t_fdf` — état global

```c
typedef struct s_fdf {
    void *mlx;       // pointeur MiniLibX
    void *win;       // fenêtre
    void *img;       // image off-screen (buffer de dessin)
    char *addr;      // adresse mémoire brute de l'image
    int   bpp;       // bits par pixel (généralement 32)
    int   line_len;  // taille d'une ligne en octets
    int   endian;    // endianness
    t_map *map;
    t_cam *cam;
    // bonus seulement :
    int color_theme; // 0=normal, 1=blanc, 2=arc-en-ciel
    int auto_rotate; // rotation automatique activée
    int seed;        // graine du générateur procédural
    int preset_id;   // preset terrain actif (0-4)
    int size_id;     // taille de map active (0-4)
} t_fdf;
```

### `t_pt` — point 2D projeté à l'écran

```c
typedef struct s_pt {
    int x;      // coordonnée écran X
    int y;      // coordonnée écran Y
    int color;  // couleur RGB du point
} t_pt;
```

### `t_vec3` — point 3D de la grille

```c
typedef struct s_vec3 {
    int col;  // colonne (axe X)
    int row;  // ligne (axe Y)
    int z;    // hauteur
} t_vec3;
```

Justification : passé par valeur dans `iso_project()` pour respecter la norme 42
(≤ 4 paramètres par fonction).

### `t_line` — état interne de Bresenham

```c
typedef struct s_line {
    int dx, dy;  // delta abs en x et y
    int sx, sy;  // direction (+1 ou -1)
    int err;     // erreur accumulée
} t_line;
```

---

## 5. Partie obligatoire — fichier par fichier

### `src/main.c`

**Rôle** : point d'entrée, initialisation, lancement de la boucle MiniLibX.

Flux d'exécution :
1. Vérifie `argc == 2`, sinon `error_exit()`.
2. Alloue `t_fdf` et `t_cam`.
3. Appelle `parse_map(argv[1])` → remplit `fdf->map`.
4. Initialise MiniLibX (`mlx_init → mlx_new_window → mlx_new_image → mlx_get_data_addr`).
5. Appelle `init_cam()` pour calculer zoom/offsets automatiquement.
6. Dessine : `draw_map()` puis `mlx_put_image_to_window()`.
7. Enregistre les hooks (`key_hook`, `close_hook`).
8. Lance `mlx_loop()` — boucle événementielle infinie.

### `src/parse.c`

**Rôle** : lire les dimensions + parser les tokens (z + couleur).

- `count_cols_and_rows(filename, &width)` :
  - Ouvre le fichier, lit ligne par ligne avec GNL.
  - Compte les lignes → `height`.
  - Compte les tokens de la 1ère ligne avec `ft_split` → `width`.
  - Retourne `height`.

- `alloc_map(map)` :
  - Alloue `map->z` et `map->color` comme tableaux 2D `int**`.
  - Utilise `ft_calloc` → tout initialisé à 0.

- `parse_token(token, &z, &color)` :
  - Cherche une virgule dans le token avec `ft_strchr`.
  - Avant la virgule → `ft_atoi` → z.
  - Après la virgule → `parse_hex_color` → couleur RGB.
  - Sans virgule → z seulement, color = `NO_COLOR (-1)`.

- `parse_hex_color(str)` :
  - Parse manuellement une chaîne hex (`0x...` ou `,0x...`).
  - Gère minuscules (a-f) et majuscules (A-F).

### `src/parse2.c`

**Rôle** : orchestration du parsing, remplissage de la grille.

- `parse_map(filename)` :
  1. Alloue `t_map`.
  2. Appelle `count_cols_and_rows` → dimensions.
  3. Appelle `alloc_map`.
  4. Ouvre le fichier une **deuxième fois**.
  5. Appelle `fill_map` (lit ligne par ligne, remplit z et color).
  6. Retourne la map.

- `fill_map(fd, map)` :
  - Initialise `z_min = INT_MAX`, `z_max = INT_MIN`.
  - Pour chaque ligne GNL → `ft_split` → `parse_row`.

- `parse_row(tokens, map, r)` :
  - Itère sur les tokens, appelle `parse_token` pour chaque case.
  - Met à jour `z_min`/`z_max` via `update_minmax`.
  - Libère tous les tokens et le tableau.

### `src/projection.c`

**Rôle** : conversion coordonnées 3D → 2D (projection isométrique).

**Formule isométrique fixe** :
```
x_screen = (col - row) × cos(30°) × zoom + x_off
y_screen = (col + row) × sin(30°) × zoom - z × z_scale + y_off
```
- `cos(30°) ≈ 0.866`, `sin(30°) = 0.5` — valeurs codées en dur.
- La vue isométrique « standard » place l'axe X vers le bas-droite et Y vers le bas-gauche.

**`init_cam(fdf)`** — calcul automatique du zoom :
- `span = width + height` (taille totale de la grille).
- Deux zooms candidats : un limité par la largeur écran, un par la hauteur.
- `zoom = min(zw, zh)` avec plancher à `1.0`.
- `z_scale = zoom / (z_max - z_min) * 20.0` → hauteur proportionnelle à la carte.
- Offsets : `x_off = WIN_W / 2`, `y_off = WIN_H / 4`.

### `src/draw.c`

**Rôle** : tracé d'un pixel, algorithme de Bresenham, conversion point 3D → t_pt.

**`put_pixel(fdf, x, y, color)`** :
- Vérifie que (x, y) est dans la fenêtre (clipping).
- Calcule l'adresse : `addr + y * line_len + x * (bpp/8)`.
- Écrit directement en mémoire (plus rapide que `mlx_pixel_put`).

**Algorithme de Bresenham** (`draw_line`) :
- Trace une ligne entre deux `t_pt` en faisant varier le pixel dominant (dx ou dy).
- Gère 8 octants grâce aux signes `sx`/`sy`.
- Interpole la couleur avec `lerp_color` à chaque étape.
- L'erreur accumulée (`err`) décide quand avancer dans la direction secondaire.

```
Etapes clés :
1. init_line : calcule dx, dy, sx, sy, err, steps.
2. Boucle : dessine le pixel courant, avance d'un pas avec step_line.
3. step_line : met à jour err et déplace p.x ou p.y selon l'erreur.
```

**`map_to_screen(fdf, col, row)`** :
- Construit un `t_vec3` avec les coords et le z de la grille.
- Appelle `iso_project`.
- Si la case a une couleur → l'utilise.
- Sinon → interpolation linéaire bleu (z_min) → blanc (z_max).

### `src/draw2.c`

**Rôle** : boucle principale de dessin.

**`draw_map(fdf)`** :
- Efface l'image (`ft_memset` à 0).
- Parcourt toutes les cases `(col, row)`.
- Pour chaque case, trace :
  - La ligne vers `(col+1, row)` si elle existe.
  - La ligne vers `(col, row+1)` si elle existe.

### `src/events.c`

**Rôle** : gestion des événements MiniLibX.

- `key_hook(key, param)` : si `KEY_ESC (65307)` → `quit()`.
- `close_hook(param)` : clic sur la croix de la fenêtre → `quit()`.
- `quit(fdf)` :
  1. Détruit l'image, la fenêtre, le display MiniLibX.
  2. Libère `mlx`, `map`, `cam`, `fdf`.
  3. `exit(0)`.

### `src/utils.c`

**Rôle** : fonctions utilitaires.

- `error_exit(msg)` : écrit sur stderr et exit(1).
- `free_map(map)` : libère le tableau 2D z, le tableau 2D color, puis la struct.
- `free_2d_array(arr, height)` : libère chaque ligne puis le tableau principal.
- `lerp_color(c1, c2, t)` :
  - Interpole linéairement chaque canal RGB séparément.
  - `t = 0.0` → c1, `t = 1.0` → c2.
  - Formule : `canal = canal1 * (1-t) + canal2 * t`.
- `ft_abs(n)` : valeur absolue entière.

---

## 6. Partie bonus — fichier par fichier

### `src/bonus/main_bonus.c`

Identique à la partie obligatoire, avec en plus :
- `srand(time(NULL))` → graine aléatoire pour le générateur.
- `mlx_mouse_hook` → zoom à la molette.
- `mlx_loop_hook` → appel de `loop_hook` à chaque frame (rotation auto).
- Champs supplémentaires initialisés : `color_theme`, `auto_rotate`, `seed`, `preset_id`, `size_id`.

### `src/bonus/projection_bonus.c`

Deux projections disponibles :

**`iso_project(pt, cam)`** — isométrique avec rotations 3D libres :
```
1. Rotation autour de Z : rot_z (dans le plan de la grille)
2. Rotation autour de X : rot_x (inclinaison avant/arrière)
3. Rotation autour de Y : rot_y (rotation latérale)
4. Application de la formule iso classique sur le point transformé
```
Utilise `cos()` et `sin()` de `<math.h>`.

**`parallel_project(pt, cam)`** — vue de dessus orthogonale :
```
x_screen = col * zoom + x_off
y_screen = row * zoom - z * z_scale + y_off
```

**`init_cam` (bonus)** :
- Initialise `rot_x = rot_y = rot_z = 0`, `proj = 0`.
- Formule de zoom similaire mais avec plancher à `0.05f` (supporte les grandes maps).
- `relief = max(20.0, span * 0.06)` → les grandes maps ont plus de relief visible.

### `src/bonus/draw2_bonus.c`

Couleur et HUD :

**`pick_color(fdf, col, row, norm)`** :
- Thème 0 (normal) : couleur de la map si définie, sinon dégradé bleu→blanc.
- Thème 1 (blanc) : `0xFFFFFF` pour tout.
- Thème 2 (arc-en-ciel) : appel à `rainbow_color(norm)`.

**`map_to_screen(fdf, col, row)`** :
- Construit un `t_vec3` avec les coords et le z de la grille.
- Appelle `iso_project` ou `parallel_project` selon `cam->proj`.
- Calcule la couleur via `pick_color`.

**`draw_hud(fdf)`** :
- Affiche en haut à gauche avec `mlx_string_put` :
  - Preset actif, taille, seed, FPS.
  - Rappel des touches de contrôle.

### `src/bonus/draw3_bonus.c`

**`draw_map(fdf)`** :
- Efface l'image (`ft_memset` à 0).
- Parcourt toutes les cases `(col, row)`.
- Pour chaque case, trace la ligne vers `(col+1, row)` et `(col, row+1)`.
- Séparé de `draw2_bonus.c` pour respecter la limite de 5 fonctions par fichier (norme 42).

### `src/bonus/color_bonus.c`

**`rainbow_color(norm)`** :
- `norm ∈ [0, 1]` → détermine la position dans le spectre visible.
- 7 couleurs de référence (rouge → violet → rouge).
- Interpolation linéaire entre la couleur i et i+1.

### `src/bonus/events_bonus.c`

- `redraw(fdf)` : `draw_map` + `mlx_put_image_to_window` + `draw_hud`.
- `loop_hook(param)` : calcule le delta de temps pour le FPS ; si `auto_rotate` actif → `rot_y += 0.02f` ; si `dirty` → redraw.
- `close_hook` / `quit` : libération complète de la mémoire + exit.

### `src/bonus/mouse_bonus.c`

Gestion des événements souris (séparé de `events_bonus.c` pour la limite norme 42 à 5 fonctions) :

- `mouse_hook(btn, x, y, param)` :
  - Clic gauche/droit → active `is_dragging` / `is_rotating`.
  - Molette haut (btn 4) → `zoom *= 1.1` (centré sur le curseur).
  - Molette bas (btn 5) → `zoom /= 1.1`.
- `release_hook` : désactive `is_dragging` / `is_rotating`.
- `motion_hook` : pan si drag, rotation si right-drag via `apply_rotation`.

### `src/bonus/keys_bonus.c`

**Tableau complet des touches** :

| Touche | Action |
|--------|--------|
| ESC | Quitter |
| Flèches | Déplacement (pan) de 20px |
| R | Reset caméra |
| W / S | Rotation X (inclinaison) |
| A / D | Rotation Y (rotation latérale) |
| Q / E | Rotation Z (rotation dans le plan) |
| + / - | Zoom in / out (×1.1) |
| [ / ] | z_scale ×1.2 ou ÷1.2 (exagération hauteur) |
| P | Basculer projection (iso ↔ parallèle) |
| C | Changer thème de couleur (3 thèmes) |
| SPACE | Activer/désactiver rotation automatique |
| 1 / 2 / 3 / 4 / 5 | Changer preset terrain (Montagne/Île/Désert/Forêt/Fjords) |
| TAB | Changer taille (100/200/300/500/1000) |
| G | Nouvelle graine aléatoire |
| N | Graine +1 (variation subtile) |
| Molette | Zoom in / out |

**`handle_mapgen`** :
- Génère un nouveau fichier `.fdf` dans `/tmp/fdf_gen.fdf`.
- Libère l'ancienne map, parse la nouvelle, réinitialise la caméra.

---

## 7. Générateur de cartes procédural (bonus avancé)

### Architecture générale

```
Graine (seed)
    ↓
perlin_init()    → table de permutation mélangée (Fisher-Yates)
    ↓
Pour chaque cellule (x, y) :
    domain_warp() ou height_at()   → bruit fractal fBm ou ridged
    apply_island_mask()            → force les bords sous l'eau
    pow(h, exponent)               → redistribution de la hauteur
    compute_temp()                 → température (latitude + altitude + bruit)
    get_biome()                    → identifie le biome
    biome_color() + compute_z()    → couleur RGB et z final
    ↓
generate_fdf_file()  → écrit dans /tmp/fdf_gen.fdf sans malloc/printf
```

### `perlin_bonus.c` — Bruit de Perlin

**`perlin_init(pn, seed)`** :
- Remplit `p[0..255]` avec les indices 0..255.
- Mélange par Fisher-Yates avec un LCG (rng = rng * 1664525 + 1013904223).
- Duplique : `p[256+i] = p[i]` pour accès sans modulo.

**`noise2d(pn, x, y)`** :
- Bruit de Perlin 2D classique.
- `fade(t) = 6t⁵ - 15t⁴ + 10t³` (courbe lisse de Ken Perlin).
- `grad2` : 4 gradients dans les 4 directions diagonales.
- Interpolation bilinéaire sur la cellule.
- Retourne une valeur dans `[-1, 1]`.

### `fbm_bonus.c` — Bruit fractal (fBm)

**`fbm(pn, x, y, cfg)`** :
- Superpose `cfg->octaves` couches de bruit.
- Chaque octave : amplitude × persistence, fréquence × lacunarity.
- Offset décorrélant `i×19.19, i×47.47` par octave → évite les artefacts de grille.
- Normalise vers `[0, 1]`.

**`ridged_fbm(pn, x, y, cfg)`** :
- Transforme `noise → 1 - |noise|` → les passages par 0 deviennent des crêtes.
- `n * n` → creuse les vallées.
- Résultat : relief alpin avec arêtes vives.

**`domain_warp(pn, x, y, cfg)`** :
- Tord les coordonnées avec deux fBm indépendants avant de lire le terrain.
- Crée des côtes irrégulières, des méandres, des fjords.

**`apply_island_mask(h, x, y, cfg)`** :
- Gradient radial : `mask = 1 - dist² * 2`.
- Le centre émerge, les bords plongent sous l'eau (SEA_LEVEL = 0.38).
- Résultat : forme d'île avec océan tout autour.

### `biome_bonus.c` — Attribution des biomes

**`compute_temp(pn, cell, cfg, h)`** :
- `latitude` : `1.0 - y / height` (nord = chaud dans le modèle).
- `altitude` : abaisse la temp selon `h * 0.4`.
- `drift` : bruit lent (échelle 0.013) → fronts climatiques.

**`get_biome(height, temp)`** :
```
h < 0.28  → DEEP_OCEAN
h < 0.38  → OCEAN
h < 0.43  → BEACH
h > 0.94  → SNOW
h > 0.86  → HIGH_MOUNTAIN
h > 0.72  → MOUNTAIN
sinon     → TUNDRA / TAIGA / PLAINS / SAVANNA / FOREST / DESERT (selon temp)
```

**`biome_color(biome, t_norm)`** :
- Deux couleurs par biome (`lo` et `hi`), interpolées selon `h` normalisé.
- Donne un dégradé naturel à l'intérieur de chaque biome.

**`compute_z(h, z_max)`** :
- Sous le niveau de la mer : `z = h / SEA_LEVEL * -5` (légèrement négatif).
- Au-dessus : `z = (h - SEA_LEVEL) / (1 - SEA_LEVEL) * z_max`.

### `mapgen_bonus.c` — Presets

| Preset | Caractéristiques |
|--------|-----------------|
| Mountain (0) | ridged fBm, 7 octaves, forte lacunarity, pas de warp → arêtes alpines |
| Island (1) | masque radial, warping 1.2, 5 octaves → île tropicale |
| Desert (2) | 3 octaves seulement, warp 1.5, z_max faible → dunes douces |
| Forest (3) | persistence élevée (0.60), warp léger → collines boisées |
| Fjords (4) | ridged + island + warp fort (4.5) → côtes déchirées nordiques |

### `mapgen_write_bonus.c` — Écriture du fichier

- Construit chaque ligne dans un buffer local de 64 Ko.
- `append_int` : écrit un entier sans printf ni malloc.
- `append_hex` : écrit `,0xRRGGBB` sans printf ni malloc.
- Un seul `write()` par ligne → I/O minimale pour les grandes maps (500x500, 1000x1000).

---

## 8. GNL — get_next_line

Fichier : `GNL/get_next_line.c`

- Utilise un tableau statique `stash[MAX_FD]` par file descriptor.
- `read_and_stash` : lit par blocs de `BUFFER_SIZE` (ici 16384), concatène dans `stash`.
- `extract_line` : extrait jusqu'au `\n` (inclus) ou fin de stash.
- `get_new_stash` : retire la ligne extraite du stash pour le prochain appel.
- Retourne `NULL` quand le fichier est épuisé.

---

## 9. MiniLibX — concepts utilisés

| Fonction | Rôle |
|----------|------|
| `mlx_init()` | Connecte au serveur X11 |
| `mlx_new_window(mlx, w, h, title)` | Crée une fenêtre |
| `mlx_new_image(mlx, w, h)` | Crée un buffer off-screen |
| `mlx_get_data_addr(img, &bpp, &ll, &end)` | Accès direct à la mémoire de l'image |
| `mlx_put_image_to_window(mlx, win, img, 0, 0)` | Affiche l'image dans la fenêtre |
| `mlx_key_hook(win, fn, param)` | Callback sur touche clavier |
| `mlx_hook(win, event, mask, fn, param)` | Callback générique (ici fermeture fenêtre event=17) |
| `mlx_mouse_hook(win, fn, param)` | Callback souris (bonus) |
| `mlx_loop_hook(mlx, fn, param)` | Callback à chaque itération de la boucle (bonus) |
| `mlx_loop(mlx)` | Démarre la boucle événementielle |
| `mlx_string_put(mlx, win, x, y, color, str)` | Affiche du texte dans la fenêtre (HUD bonus) |
| `mlx_destroy_image` | Libère l'image |
| `mlx_destroy_window` | Ferme la fenêtre |
| `mlx_destroy_display` | Déconnecte le display X11 |

**Écriture pixel directe** (plus rapide que `mlx_pixel_put`) :
```c
dst = addr + (y * line_len + x * (bpp / 8));
*(unsigned int *)dst = color;
```

---

## 10. Algorithme de Bresenham — explication pas à pas

Objectif : tracer une ligne entre P1 et P2 sans flottants, en utilisant uniquement des additions.

```
Exemple : P1=(0,0) → P2=(5,2)
dx=5, dy=2, sx=1, sy=1, err=dx-dy=3

Itération 0 : dessine (0,0)  ; e2=6  > -2 → x++ ; e2=6  < 5 : NON
Itération 1 : dessine (1,0)  ; e2=6  > -2 → x++ ; e2=6  < 5 : NON
Itération 2 : dessine (2,0)  ; e2=6  > -2 → x++ ; e2=6  < 5 : NON → err=err-dy=1
Itération 3 : dessine (3,0)  ; e2=2  > -2 → x++ ; e2=2  < 5 → y++ err=err+dx=6
...
```

La clé : `err` mesure combien on s'écarte de la droite idéale. Quand il dépasse un seuil, on avance dans la direction secondaire.

---

## 11. Gestion mémoire

- La map est allouée en 2 passes : d'abord `int **` (tableau de pointeurs), puis `int *` pour chaque ligne.
- Libération dans `free_map` : boucle sur les lignes, puis libère le tableau principal, puis la struct.
- En cas d'erreur d'allocation → `error_exit` → `exit(1)` (pas de fuite détectable).
- Dans le bonus, `handle_mapgen` libère l'ancienne map avant d'en charger une nouvelle.

---

## 12. Questions fréquentes en correction

**Q : Pourquoi le fichier est ouvert deux fois dans `parse_map` ?**  
A : La première passe (`count_cols_and_rows`) compte les dimensions. La deuxième passe (`fill_map`) remplit la grille. On ne peut pas revenir en arrière avec GNL sans stocker tout en RAM.

**Q : Pourquoi `t_vec3` passé par valeur ?**  
A : La norme 42 limite à 4 paramètres par fonction. Regrouper col/row/z en struct permet de passer le point en 1 paramètre tout en restant < 4 au total.

**Q : Comment le zoom est-il calculé automatiquement ?**  
A : `span = width + height`. On calcule le zoom qui fait tenir la diagonale isométrique dans WIN_W et dans WIN_H. On prend le minimum pour que la map tienne dans les deux dimensions.

**Q : Pourquoi `ft_memset` sur toute l'image avant de dessiner ?**  
A : Sans ça, les frames précédentes resteraient visibles lors du redraw (bonus). Ça efface en noir (0x000000).

**Q : Différence entre `mlx_pixel_put` et l'accès direct ?**  
A : `mlx_pixel_put` génère un appel système (flush X11) à chaque pixel → catastrophiquement lent pour une image entière. L'accès direct via `addr` écrit en RAM, et un seul `mlx_put_image_to_window` affiche tout d'un coup.

**Q : Pourquoi `bpp / 8` dans le calcul d'adresse pixel ?**  
A : `bpp` est en **bits** (typiquement 32). On divise par 8 pour avoir l'offset en **octets** par pixel.

**Q : Comment fonctionne `lerp_color` ?**  
A : Interpolation linéaire canal par canal :  
`R = R1 * (1-t) + R2 * t`  
`G = G1 * (1-t) + G2 * t`  
`B = B1 * (1-t) + B2 * t`  
Résultat réassemblé : `(R << 16) | (G << 8) | B`.

**Q : Quelle est la formule isométrique exacte ?**  
A :  
```
x_screen = (col - row) × 0.866 × zoom + x_off
y_screen = (col + row) × 0.5  × zoom - z × z_scale + y_off
```
`0.866 = cos(30°)`, `0.5 = sin(30°)`.

**Q : Qu'est-ce que le domain warping ?**  
A : Au lieu de lire `noise(x, y)`, on lit `noise(x + fbm(x,y), y + fbm(x+5.2, y+1.3))`. Le bruit tord les coordonnées → formes organiques, côtes irrégulières.

**Q : Qu'est-ce que le bruit ridged ?**  
A : `n = 1 - |noise|`. Les passages par zéro du bruit normal (transitions douces) deviennent des crêtes vives. `n*n` accentue les vallées → profil alpin.

**Q : Pourquoi écrire dans `/tmp/fdf_gen.fdf` et non directement en RAM ?**  
A : Le parseur de map existant lit depuis un fichier (fd). Réutiliser `parse_map` sans le modifier évite la duplication de code et reste conforme à la norme.

---

## 13. Commandes de test rapide

```bash
# Partie obligatoire
./fdf maps/test_maps/elem.fdf
./fdf maps/test_maps/42.fdf
./fdf maps/test_maps/mars.fdf
./fdf maps/test_maps/elem-col.fdf   # test couleurs
./fdf maps/test_maps/julia.fdf
./fdf maps/test_maps/100-6.fdf      # grande map

# Partie bonus
./fdf_bonus maps/test_maps/mars.fdf
# puis : WASD pour rotation, +/- zoom, [ ] z_scale, P projection, C couleur
# SPACE rotation auto, R reset, 1-5 générer terrain

# Valgrind (si disponible)
valgrind --leak-check=full ./fdf maps/test_maps/elem.fdf
```

---

## 14. Structure des dossiers

```
fdf/
├── Makefile
├── include/
│   ├── fdf.h              ← structs + prototypes partie obligatoire
│   └── bonus/
│       ├── fdf_bonus.h    ← structs + prototypes bonus
│       └── mapgen_bonus.h ← structs + prototypes générateur
├── src/
│   ├── main.c
│   ├── parse.c / parse2.c
│   ├── projection.c
│   ├── draw.c / draw2.c
│   ├── events.c
│   ├── utils.c
│   └── bonus/
│       ├── main_bonus.c
│       ├── parse_bonus.c / parse2_bonus.c
│       ├── projection_bonus.c
│       ├── draw_bonus.c / draw2_bonus.c / draw3_bonus.c
│       ├── events_bonus.c
│       ├── mouse_bonus.c
│       ├── keys_bonus.c
│       ├── color_bonus.c
│       ├── utils_bonus.c
│       ├── screenshot_bonus.c
│       ├── perlin_bonus.c
│       ├── fbm_bonus.c
│       ├── biome_bonus.c
│       ├── mapgen_bonus.c
│       └── mapgen_write_bonus.c
├── GNL/
│   ├── get_next_line.c
│   ├── get_next_line_utils.c
│   └── get_next_line.h
├── libft/                 ← libft compilée en libft.a
├── minilibx-linux/        ← MiniLibX compilée en libmlx.a
└── maps/test_maps/        ← cartes de test fournies
```
