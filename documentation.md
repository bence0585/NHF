# NHF-Farm Programozói Dokumentáció

## Bevezetés

Ez a dokumentáció bemutatja a NHF-Farm program belső felépítését és működését. A cél az, hogy egy másik programozó könnyen megértse a program felépítését és működését, valamint eligazodjon az ismeretlen forráskódban.

## Megvalósított Módszerek

A program fő célja egy rács alapú játék megvalósítása, amelyben a felhasználó egy karaktert irányít, és különböző műveleteket végezhet a rácson. A program SDL3 könyvtárat használ a grafikus megjelenítéshez és eseménykezeléshez.

## Adatszerkezetek

- `grid[32][32]`: A rács állapotát tároló kétdimenziós tömb. Minden elem egy tile típusát jelöli.
- `SDL_Texture`: Textúrák tárolására szolgál, például a karakter és a tilemap textúrái.

## Modulok és Függvények

### Fő Modulok

- `main.c`: A program belépési pontja. Inicializálja az SDL-t, létrehozza az ablakot és a renderert, majd elindítja az eseménykezelő ciklust.
- `init.c`: Az SDL inicializálásáért, az ablak és a renderer létrehozásáért, valamint a takarításért felelős függvényeket tartalmazza.
- `grid.c`: A rács állapotának betöltéséért és kirajzolásáért felelős függvényeket tartalmazza.
- `texture.c`: Textúrák betöltéséért felelős függvényeket tartalmazza.
- `event_loop.c`: Az eseménykezelő ciklust és a felhasználói interakciókat kezeli.
- `ui.c`: A felhasználói felület elemeinek kirajzolásáért és az interakciók kezeléséért felelős függvényeket tartalmazza.

### Függvények

#### `main.c`

- `int main(int argc, char *argv[])`: A program belépési pontja.

#### `init.c`

- `int initialize_SDL()`: Inicializálja az SDL könyvtárat.
- `SDL_Window *create_window(const char *title, int width, int height)`: Létrehozza az SDL ablakot.
- `SDL_Renderer *create_renderer(SDL_Window *window)`: Létrehozza az SDL renderert.
- `void cleanup(SDL_Renderer *renderer, SDL_Window *window)`: Takarítja az SDL erőforrásokat.

#### `grid.c`

- `void read_grid_state(const char *filename)`: Betölti a rács állapotát egy fájlból.
- `void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`: Kirajzolja a rácsot.
- `void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)`: Átkonvertálja a karakter koordinátáit rács koordinátákra.
- `void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`: Kiemeli a rács egy négyzetét.

#### `texture.c`

- `SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path)`: Betölt egy textúrát a megadott fájlból.

#### `event_loop.c`

- `void event_loop(SDL_Renderer *renderer)`: Az eseménykezelő ciklus.

#### `ui.c`

- `void render_ui(SDL_Renderer *renderer)`: Kirajzolja a felhasználói felület elemeit.
- `bool is_zoom_in_button_clicked(int x, int y)`: Ellenőrzi, hogy a nagyítás gombra kattintottak-e.
- `bool is_zoom_out_button_clicked(int x, int y)`: Ellenőrzi, hogy a kicsinyítés gombra kattintottak-e.

## Függvények Paraméterei és Visszatérési Értékei

### `initialize_SDL()`

- **Paraméterek**: Nincs
- **Visszatérési érték**: `int` - 0, ha sikeres, egyébként negatív érték hibakód esetén.

### `create_window(const char *title, int width, int height)`

- **Paraméterek**:
  - `title`: Az ablak címe.
  - `width`: Az ablak szélessége.
  - `height`: Az ablak magassága.
- **Visszatérési érték**: `SDL_Window*` - Az ablak mutatója, vagy NULL hiba esetén.

### `create_renderer(SDL_Window *window)`

- **Paraméterek**:
  - `window`: Az ablak mutatója, amelyhez a renderert létrehozzuk.
- **Visszatérési érték**: `SDL_Renderer*` - A renderer mutatója, vagy NULL hiba esetén.

### `cleanup(SDL_Renderer *renderer, SDL_Window *window)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
  - `window`: Az ablak mutatója.
- **Visszatérési érték**: Nincs

### `read_grid_state(const char *filename)`

- **Paraméterek**:
  - `filename`: A fájl neve, amelyből a rács állapotát betöltjük.
- **Visszatérési érték**: Nincs

### `render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
  - `tilemap`: A tilemap textúra mutatója.
  - `tilemap_width`: A tilemap szélessége.
  - `tilemap_height`: A tilemap magassága.
  - `zoom_level`: A nagyítás mértéke.
  - `offset_x`: Az x tengelyen eltolás.
  - `offset_y`: Az y tengelyen eltolás.
- **Visszatérési érték**: Nincs

### `convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)`

- **Paraméterek**:
  - `character_x`: A karakter x koordinátája.
  - `character_y`: A karakter y koordinátája.
  - `tile_size`: A rács mérete.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: Nincs

### `highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
  - `grid_x`: A négyzet x koordinátája a rácsban.
  - `grid_y`: A négyzet y koordinátája a rácsban.
  - `tile_size`: A négyzet mérete.
  - `zoom_level`: A nagyítás mértéke.
  - `offset_x`: Az x tengelyen eltolás.
  - `offset_y`: Az y tengelyen eltolás.
- **Visszatérési érték**: Nincs

### `load_texture(SDL_Renderer *renderer, const char *file_path)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
  - `file_path`: A fájl útvonala.
- **Visszatérési érték**: `SDL_Texture*` - A textúra mutatója, vagy NULL hiba esetén.

### `event_loop(SDL_Renderer *renderer)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
- **Visszatérési érték**: Nincs

### `render_ui(SDL_Renderer *renderer)`

- **Paraméterek**:
  - `renderer`: A renderer mutatója.
- **Visszatérési érték**: Nincs

### `is_zoom_in_button_clicked(int x, int y)`

- **Paraméterek**:
  - `x`: Az egér x koordinátája.
  - `y`: Az egér y koordinátája.
- **Visszatérési érték**: `bool` - Igaz, ha a nagyítás gombra kattintottak, egyébként hamis.

### `is_zoom_out_button_clicked(int x, int y)`

- **Paraméterek**:
  - `x`: Az egér x koordinátája.
  - `y`: Az egér y koordinátája.
- **Visszatérési érték**: `bool` - Igaz, ha a kicsinyítés gombra kattintottak, egyébként hamis.

## Példák

### `initialize_SDL()`

```c
if (initialize_SDL() < 0) {
    return -1;
}
```

### `create_window(const char *title, int width, int height)`

```c
SDL_Window *window = create_window("Ablak", 1280, 720);
if (window == NULL) {
    return -2;
}
```

### `create_renderer(SDL_Window *window)`

```c
SDL_Renderer *renderer = create_renderer(window);
if (renderer == NULL) {
    return -3;
}
```

### `cleanup(SDL_Renderer *renderer, SDL_Window *window)`

```c
cleanup(renderer, window);
```

### `read_grid_state(const char *filename)`

```c
read_grid_state("../src/grid_state.txt");
```

### `render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`

```c
render_grid(renderer, tilemap, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);
```

### `convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)`

```c
int grid_x, grid_y;
convert_to_grid_coordinates(character_x, character_y, tile_size, &grid_x, &grid_y);
```

### `highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`

```c
highlight_grid_square(renderer, grid_x, grid_y, tile_size, zoom_level, offset_x, offset_y);
```

### `load_texture(SDL_Renderer *renderer, const char *file_path)`

```c
SDL_Texture *texture = load_texture(renderer, "../src/img/texture.png");
```

### `event_loop(SDL_Renderer *renderer)`

```c
event_loop(renderer);
```

### `render_ui(SDL_Renderer *renderer)`

```c
render_ui(renderer);
```

### `is_zoom_in_button_clicked(int x, int y)`

```c
if (is_zoom_in_button_clicked(x, y)) {
    zoom_level += 1;
}
```

### `is_zoom_out_button_clicked(int x, int y)`

```c
if (is_zoom_out_button_clicked(x, y)) {
    zoom_level -= 1;
}
```
