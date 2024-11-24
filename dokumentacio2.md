# NHF-Farm Programozói Dokumentáció

## Modulok és Függvények

### Fő Modulok

- `character.c`: A karakter inicializálásáért, mozgásáért, animációjáért és interakcióiért felelős függvényeket tartalmazza.

### Függvények

#### `character.c`

- `void initialize_character(Character *character, int start_x, int start_y)`: Inicializálja a karaktert a megadott kezdőpozícióval.
- `void update_character_tile(Character *character, int tile_size)`: Frissíti a karakter csempe koordinátáit a karakter x és y pozíciója alapján.
- `void handle_character_movement(const bool *state, Character *character, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height)`: Kezeli a karakter mozgását a megadott állapotok alapján.
- `void calculate_look_coordinates(Character *character, int tile_size)`: Kiszámítja a karakter nézési koordinátáit a karakter nézési iránya alapján.
- `void update_look_tile(Character *character, int tile_size)`: Frissíti a karakter nézési csempéjét a karakter nézési koordinátái alapján.
- `void update_animation(AnimationController *anim_ctrl)`: Frissíti az animációt az animációvezérlő alapján.
- `void handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info)`: Kezeli a billentyű lenyomás eseményeket.
- `void handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection)`: Kezeli az egér görgő eseményeket.
- `void handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level)`: Kezeli az egér gombok lenyomás eseményeket.
- `void update_character_position(Character *character, Grid *grid, int grid_width, int grid_height, int tile_size, int character_tile_width, int character_tile_height)`: Frissíti a karakter pozícióját, hogy ne hagyja el a rácsot.

## Függvények Paraméterei és Visszatérési Értékei

### `initialize_character(Character *character, int start_x, int start_y)`

- **Paraméterek**:
  - `character`: A karakter mutatója.
  - `start_x`: A kezdő x pozíció.
  - `start_y`: A kezdő y pozíció.
- **Visszatérési érték**: Nincs

### `update_character_tile(Character *character, int tile_size)`

- **Paraméterek**:
  - `character`: A karakter mutatója.
  - `tile_size`: A csempe mérete.
- **Visszatérési érték**: Nincs

### `handle_character_movement(const bool *state, Character *character, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height)`

- **Paraméterek**:
  - `state`: A billentyűállapotok.
  - `character`: A karakter mutatója.
  - `grid`: A rács mutatója.
  - `movement_speed`: A mozgási sebesség.
  - `tile_size`: A csempe mérete.
  - `character_tile_width`: A karakter csempe szélessége.
  - `character_tile_height`: A karakter csempe magassága.
- **Visszatérési érték**: Nincs

### `calculate_look_coordinates(Character *character, int tile_size)`

- **Paraméterek**:
  - `character`: A karakter mutatója.
  - `tile_size`: A csempe mérete.
- **Visszatérési érték**: Nincs

### `update_look_tile(Character *character, int tile_size)`

- **Paraméterek**:
  - `character`: A karakter mutatója.
  - `tile_size`: A csempe mérete.
- **Visszatérési érték**: Nincs

### `update_animation(AnimationController *anim_ctrl)`

- **Paraméterek**:
  - `anim_ctrl`: Az animációvezérlő mutatója.
- **Visszatérési érték**: Nincs

### `handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `quit`: A kilépési változó.
  - `show_debug_info`: A debug információ megjelenítése.
- **Visszatérési érték**: Nincs

### `handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `zoom_level`: A nagyítási szint.
- **Visszatérési érték**: Nincs

### `update_character_position(Character *character, Grid *grid, int grid_width, int grid_height, int tile_size, int character_tile_width, int character_tile_height)`

- **Paraméterek**:
  - `character`: A karakter mutatója.
  - `grid`: A rács mutatója.
  - `grid_width`: A rács szélessége.
  - `grid_height`: A rács magassága.
  - `tile_size`: A csempe mérete.
  - `character_tile_width`: A karakter csempe szélessége.
  - `character_tile_height`: A karakter csempe magassága.
- **Visszatérési érték**: Nincs

- `crop.c`: A növények inicializálásáért, frissítéséért és kirajzolásáért felelős függvényeket tartalmazza.

### Függvények

#### `crop.c`

- `void initialize_crop_manager(CropManager *crop_manager)`: Inicializálja a növénykezelőt.
- `void add_crop(CropManager *crop_manager, int x, int y, CropType type, int growth_time)`: Hozzáad egy növényt a növénykezelőhöz.
- `void update_crops(CropManager *crop_manager, int ticks, Grid *grid)`: Frissíti a növények állapotát a megadott időegységek alapján.
- `void render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y)`: Rendereli a növényeket a megadott paraméterek alapján.

## Függvények Paraméterei és Visszatérési Értékei

### `initialize_crop_manager(CropManager *crop_manager)`

- **Paraméterek**:
  - `crop_manager`: A növénykezelő mutatója.
- **Visszatérési érték**: Nincs

### `add_crop(CropManager *crop_manager, int x, int y, CropType type, int growth_time)`

- **Paraméterek**:
  - `crop_manager`: A növénykezelő mutatója.
  - `x`: Az x koordináta.
  - `y`: Az y koordináta.
  - `type`: A növény típusa.
  - `growth_time`: A növekedési idő.
- **Visszatérési érték**: Nincs

### `update_crops(CropManager *crop_manager, int ticks, Grid *grid)`

- **Paraméterek**:
  - `crop_manager`: A növénykezelő mutatója.
  - `ticks`: Az időegységek száma.
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `crop_texture`: A növény textúra.
  - `crop_manager`: A növénykezelő mutatója.
  - `tile_size`: A csempe mérete.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
- **Visszatérési érték**: Nincs

- `event_loop.c`: Az eseménykezelő ciklust és a felhasználói interakciókat kezeli.

### Függvények

#### `event_loop.c`

- `const char *get_tile_type_name(TileType tile_type)`: Visszaadja a megadott csempe típus nevét.
- `void render_shadow(SDL_Renderer *renderer, int x, int y, int radius)`: Árnyékot rajzol a megadott pozícióra adott sugárral.
- `void game_tick(CropManager *crop_manager, int ticks, Grid *grid)`: Tick rendszer frissítése a játékban.
- `void handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info)`: Billentyű lenyomás esemény kezelése.
- `void handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection)`: Egér görgő esemény kezelése.
- `void handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level)`: Egér gombok lenyomás esemény kezelése.
- `void render_debug_info(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height)`: Debug információk kirajzolása.
- `void initialize_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid **grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int *grid_width, int *grid_height)`: Játék inicializálása.
- `void render_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, SDL_Texture *tilemap, SDL_Texture *character_tileset, SDL_Texture *item_tilemap, SDL_Texture *crop_texture, int screen_width, int screen_height, int zoom_level, int fps, bool show_debug_info)`: Játék kirajzolása.
- `void handle_events(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info, int screen_width, int screen_height, int *zoom_level)`: Események kezelése.
- `void event_loop(SDL_Renderer *renderer, Grid *background_grid, ForegroundGrid *foreground_grid)`: Az eseménykezelő ciklus.

## Függvények Paraméterei és Visszatérési Értékei

### `const char *get_tile_type_name(TileType tile_type)`

- **Paraméterek**:
  - `tile_type`: A csempe típusa.
- **Visszatérési érték**: `const char*` - A csempe típus neve.

### `void render_shadow(SDL_Renderer *renderer, int x, int y, int radius)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `x`: Az x koordináta.
  - `y`: Az y koordináta.
  - `radius`: A sugár.
- **Visszatérési érték**: Nincs

### `void game_tick(CropManager *crop_manager, int ticks, Grid *grid)`

- **Paraméterek**:
  - `crop_manager`: A növénykezelő mutatója.
  - `ticks`: Az időegységek száma.
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `void handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `quit`: A kilépési változó.
  - `show_debug_info`: A debug információ megjelenítése.
- **Visszatérési érték**: Nincs

### `void handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `void handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `zoom_level`: A nagyítási szint.
- **Visszatérési érték**: Nincs

### `void render_debug_info(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
- **Visszatérési érték**: Nincs

### `void initialize_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid **grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int *grid_width, int *grid_height)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `grid_width`: A rács szélessége.
  - `grid_height`: A rács magassága.
- **Visszatérési érték**: Nincs

### `void render_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, SDL_Texture *tilemap, SDL_Texture *character_tileset, SDL_Texture *item_tilemap, SDL_Texture *crop_texture, int screen_width, int screen_height, int zoom_level, int fps, bool show_debug_info)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `tilemap`: A csempetérkép textúra.
  - `character_tileset`: A karakter textúra.
  - `item_tilemap`: A tárgy textúra.
  - `crop_texture`: A növény textúra.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `zoom_level`: A nagyítási szint.
  - `fps`: A képkocka per másodperc.
  - `show_debug_info`: A debug információ megjelenítése.
- **Visszatérési érték**: Nincs

### `void handle_events(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info, int screen_width, int screen_height, int *zoom_level)`

- **Paraméterek**:
  - `event`: Az esemény mutatója.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `quit`: A kilépési változó.
  - `show_debug_info`: A debug információ megjelenítése.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `zoom_level`: A nagyítási szint.
- **Visszatérési érték**: Nincs

### `void event_loop(SDL_Renderer *renderer, Grid *background_grid, ForegroundGrid *foreground_grid)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `background_grid`: A háttér rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
- **Visszatérési érték**: Nincs

- `game_state.c`: A játékállapot és a növényállapot mentéséért és betöltéséért felelős függvényeket tartalmazza.

### Függvények

#### `game_state.c`

- `void save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection)`: Mentési fájl létrehozása és a játékállapot mentése.
- `void load_game_state(const char *filename, int *character_x, int *character_y, InventorySelection *inventory_selection)`: Betölti a játékállapotot a mentési fájlból.
- `void save_crop_state(const char *filename, CropManager *crop_manager)`: Mentési fájl létrehozása és a növényállapot mentése.
- `void load_crop_state(const char *filename, CropManager *crop_manager)`: Betölti a növényállapotot a mentési fájlból.

## Függvények Paraméterei és Visszatérési Értékei

### `save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `character_x`: A karakter x koordinátája.
  - `character_y`: A karakter y koordinátája.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `load_game_state(const char *filename, int *character_x, int *character_y, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `character_x`: A karakter x koordinátájának mutatója.
  - `character_y`: A karakter y koordinátájának mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `save_crop_state(const char *filename, CropManager *crop_manager)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `crop_manager`: A növénykezelő mutatója.
- **Visszatérési érték**: Nincs

### `load_crop_state(const char *filename, CropManager *crop_manager)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `crop_manager`: A növénykezelő mutatója.
- **Visszatérési érték**: Nincs

- `grid.c`: A rács állapotának létrehozásáért, betöltéséért, kirajzolásáért és kezeléséért felelős függvényeket tartalmazza.

### Függvények

#### `grid.c`

- `Grid *create_grid(int width, int height)`: Létrehoz egy rácsot a megadott szélességgel és magassággal.
- `void destroy_grid(Grid *grid)`: Felszabadítja a rácsot és annak összes celláját.
- `void determine_grid_size(const char *filename, int *width, int *height)`: Meghatározza a rács szélességét és magasságát a megadott fájl alapján.
- `void read_grid_state(const char *filename, Grid *grid)`: Beolvassa a rács állapotát a megadott fájlból.
- `void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`: Rács kirajzolása a megadott rajzolóra a megadott csempetérképpel.
- `void render_visible_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y, int screen_width, int screen_height)`: A látható rács kirajzolása a megadott rajzolóra a megadott csempetérképpel.
- `void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)`: A rács celláinak koordinátáit átalakítja rács koordinátákra.
- `void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`: A rács cella amin a játékos állt, kirajzolása.
- `void highlight_look_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`: Kijelöli a rács cellát, ahol a játékos néz.
- `void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type)`: A rács celláinak típusát beállítja a megadott értékre.
- `TileType get_tile_type(Grid *grid, int grid_x, int grid_y)`: A rács celláinak típusát lekéri.
- `void update_tile_texture(Grid *grid, int grid_x, int grid_y)`: A rács celláinak optikai és fizikai rétegét beállítja a megadott értékre.
- `ForegroundGrid *create_foreground_grid(int width, int height)`: A háttér fölé kirajzolt réteg létrehozása.
- `void destroy_foreground_grid(ForegroundGrid *grid)`: A háttér fölé kirajzolt réteg felszabadítása.
- `void render_foreground_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, ForegroundGrid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`: A háttér fölé kirajzolt réteg kirajzolása a megadott rajzolóra a megadott csempetérképpel.
- `bool check_collision(ForegroundGrid *grid, int grid_x, int grid_y)`: Az ütközés ellenőrzése adott rács koordinátákon.
- `void read_foreground_grid_state(const char *filename, ForegroundGrid *grid)`: Az előtér beolvasása a megadott fájlból.
- `void read_collision_data(const char *filename, Grid *grid)`: Az ütközési adatok beolvasása a megadott fájlból.
- `void toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y)`: Ütközési adatok automatikus generálása a háttér és előtér rétegek alapján.
- `void save_grid_state(const char *filename, Grid *grid)`: Háttér rács adatainak mentése.
- `void save_foreground_grid_state(const char *filename, ForegroundGrid *grid)`: Előtér rács adatainak mentése.
- `void clear_foreground_grid(ForegroundGrid *grid)`: Előtér rács letisztítása.
- `void update_collision_data(const char *filename, Grid *grid, ForegroundGrid *fg_grid)`: Ütközési adatok frissítése a háttér és előtér rétegek alapján.

## Függvények Paraméterei és Visszatérési Értékei

### `create_grid(int width, int height)`

- **Paraméterek**:
  - `width`: A rács szélessége.
  - `height`: A rács magassága.
- **Visszatérési érték**: `Grid*` - A rács mutatója, vagy NULL hiba esetén.

### `destroy_grid(Grid *grid)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `determine_grid_size(const char *filename, int *width, int *height)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `width`: A rács szélességének mutatója.
  - `height`: A rács magasságának mutatója.
- **Visszatérési érték**: Nincs

### `read_grid_state(const char *filename, Grid *grid)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `tilemap`: A csempetérkép textúrája.
  - `grid`: A rács mutatója.
  - `tilemap_width`: A csempetérkép szélessége.
  - `tilemap_height`: A csempetérkép magassága.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
- **Visszatérési érték**: Nincs

### `render_visible_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y, int screen_width, int screen_height)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `tilemap`: A csempetérkép textúrája.
  - `grid`: A rács mutatója.
  - `tilemap_width`: A csempetérkép szélessége.
  - `tilemap_height`: A csempetérkép magassága.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
- **Visszatérési érték**: Nincs

### `convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)`

- **Paraméterek**:
  - `character_x`: A karakter x koordinátája.
  - `character_y`: A karakter y koordinátája.
  - `tile_size`: A csempe mérete.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: Nincs

### `highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `tile_size`: A csempe mérete.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
- **Visszatérési érték**: Nincs

### `highlight_look_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `tile_size`: A csempe mérete.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
- **Visszatérési érték**: Nincs

### `set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `tile_type`: A cella típusa.
- **Visszatérési érték**: Nincs

### `get_tile_type(Grid *grid, int grid_x, int grid_y)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: `TileType` - A cella típusa.

### `update_tile_texture(Grid *grid, int grid_x, int grid_y)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: Nincs

### `create_foreground_grid(int width, int height)`

- **Paraméterek**:
  - `width`: A réteg szélessége.
  - `height`: A réteg magassága.
- **Visszatérési érték**: `ForegroundGrid*` - A réteg mutatója, vagy NULL hiba esetén.

### `destroy_foreground_grid(ForegroundGrid *grid)`

- **Paraméterek**:
  - `grid`: A réteg mutatója.
- **Visszatérési érték**: Nincs

### `render_foreground_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, ForegroundGrid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)`

- **Paraméterek**:
  - `renderer`: A rajzoló mutatója.
  - `tilemap`: A csempetérkép textúrája.
  - `grid`: A réteg mutatója.
  - `tilemap_width`: A csempetérkép szélessége.
  - `tilemap_height`: A csempetérkép magassága.
  - `zoom_level`: A nagyítási szint.
  - `offset_x`: Az x eltolás.
  - `offset_y`: Az y eltolás.
- **Visszatérési érték**: Nincs

### `check_collision(ForegroundGrid *grid, int grid_x, int grid_y)`

- **Paraméterek**:
  - `grid`: A réteg mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: `bool` - Igaz, ha ütközés van, egyébként hamis.

### `read_foreground_grid_state(const char *filename, ForegroundGrid *grid)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `grid`: Az előtér mutatója.
- **Visszatérési érték**: Nincs

### `read_collision_data(const char *filename, Grid *grid)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `grid`: A rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
- **Visszatérési érték**: Nincs

### `save_grid_state(const char *filename, Grid *grid)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `grid`: A rács mutatója.
- **Visszatérési érték**: Nincs

### `save_foreground_grid_state(const char *filename, ForegroundGrid *grid)`

- **Paraméterek**:
  - `filename`: A mentési fájl neve.
  - `grid`: Az előtér mutatója.
- **Visszatérési érték**: Nincs

### `clear_foreground_grid(ForegroundGrid *grid)`

- **Paraméterek**:
  - `grid`: Az előtér mutatója.
- **Visszatérési érték**: Nincs

### `update_collision_data(const char *filename, Grid *grid, ForegroundGrid *fg_grid)`

- **Paraméterek**:
  - `filename`: A fájl neve.
  - `grid`: A háttér réteg mutatója.
  - `fg_grid`: Az előtér réteg mutatója.
- **Visszatérési érték**: Nincs

- `init.c`: Az SDL inicializálásáért, az ablak és a renderer létrehozásáért, valamint a takarításért felelős függvényeket tartalmazza.

### Függvények

#### `init.c`

- `int initialize_SDL()`: Inicializálja az SDL könyvtárat.
- `int initialize_SDLTTF()`: Inicializálja az SDL_ttf könyvtárat.
- `SDL_Window *create_window(const char *title, int width, int height)`: Létrehozza az SDL ablakot.
- `SDL_Renderer *create_renderer(SDL_Window *window)`: Létrehozza az SDL renderert.
- `void cleanup(SDL_Renderer *renderer, SDL_Window *window)`: Takarítja az SDL erőforrásokat.

## Függvények Paraméterei és Visszatérési Értékei

### `initialize_SDL()`

- **Paraméterek**: Nincs
- **Visszatérési érték**: `int` - 0, ha sikeres, egyébként negatív érték hibakód esetén.

### `initialize_SDLTTF()`

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

- `texture.c`: Textúrák betöltéséért felelős függvényeket tartalmazza.

### Függvények

#### `texture.c`

- `SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path)`: Betölt egy textúrát a megadott fájlból.

## Függvények Paraméterei és Visszatérési Értékei

### `load_texture(SDL_Renderer *renderer, const char *file_path)`

- **Paraméterek**:
  - `renderer`: A kirajzolást végző renderer.
  - `file_path`: A fájl útvonala.
- **Visszatérési érték**: `SDL_Texture*` - Az SDL_Texture mutatója, vagy NULL, ha sikertelen.

- `tool.c`: Az eszközök használatáért, növények ültetéséért és betakarításáért, valamint a bolt műveletek kezeléséért felelős függvényeket tartalmazza.

### Függvények

#### `tool.c`

- `void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`: Kezeli az eszköz műveleteket a megadott rács koordinátákon.
- `void handle_crop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`: Kezeli a növény ültetését a megadott rács koordinátákon.
- `void handle_harvest_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`: Kezeli a növény betakarítását a megadott rács koordinátákon.
- `void handle_shop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, InventorySelection *inventory_selection)`: Kezeli a bolt műveleteit a megadott rács koordinátákon.

## Függvények Paraméterei és Visszatérési Értékei

### `handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `tool`: Az eszköz típusa.
  - `grid`: A rács mutatója.
  - `fg_grid`: Az előtér rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `crop_manager`: A növénykezelő mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `handle_crop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `fg_grid`: Az előtér rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `crop_manager`: A növénykezelő mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `handle_harvest_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `fg_grid`: Az előtér rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `crop_manager`: A növénykezelő mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

### `handle_shop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, InventorySelection *inventory_selection)`

- **Paraméterek**:
  - `grid`: A rács mutatója.
  - `fg_grid`: Az előtér rács mutatója.
  - `grid_x`: A rács x koordinátája.
  - `grid_y`: A rács y koordinátája.
  - `inventory_selection`: Az inventárválasztó mutatója.
- **Visszatérési érték**: Nincs

- `ui.c`: A felhasználói felület elemeinek kirajzolásáért és az interakciók kezeléséért felelős függvényeket tartalmazza.

### Függvények

#### `ui.c`

- `int initialize_font(const char *font_path, int font_size)`: Inicializálja a betűtípust.
- `void render_text(SDL_Renderer *renderer, const char *text, SDL_Color color, float x, float y, float w, float h)`: Kirajzol egy szöveget a megadott helyre és méretben a megadott színnel és betűtípussal.
- `void cleanup_font()`: Betűtípus felszabadítása.
- `void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height, int inventory_y, int item_offset)`: A karakterláda kirajzolása.
- `void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height)`: A felhasználói felület kirajzolása.
- `void render_button(SDL_Renderer *renderer, ButtonType button)`: Gomb kirajzolása.
- `bool is_button_clicked(ButtonType button, int x, int y)`: Megvizsgálja, hogy a gombra kattintottak-e.
- `bool is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot)`: Megvizsgálja, hogy az inventárió slotra kattintottak-e.
- `void handle_aux_selection(int *selected_aux_item, int inventory_size, int offset)`: Az aux karakterláda kiválasztásának kezelése.
- `void render_debug_info(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height)`: Debug információk kirajzolása.

## Függvények Paraméterei és Visszatérési Értékei

### `initialize_font(const char *font_path, int font_size)`

- **Paraméterek**:
  - `font_path`: A betűtípus elérési útja.
  - `font_size`: A betűméret.
- **Visszatérési érték**: `int` - 0, ha sikeres, -1, ha sikertelen.

### `render_text(SDL_Renderer *renderer, const char *text, SDL_Color color, float x, float y, float w, float h)`

- **Paraméterek**:
  - `renderer`: A rajzoló.
  - `text`: A szöveg.
  - `color`: A szín.
  - `x`: X koordináta.
  - `y`: Y koordináta.
  - `w`: Szélesség.
  - `h`: Magasság.
- **Visszatérési érték**: Nincs

### `cleanup_font()`

- **Paraméterek**: Nincs
- **Visszatérési érték**: Nincs

### `render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height, int inventory_y, int item_offset)`

- **Paraméterek**:
  - `renderer`: A rajzoló.
  - `item_tilemap`: A tárgyak textúrája.
  - `inventory_selection`: A kiválasztott tárgyak.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `inventory_y`: Az karakterláda y koordinátája, a képernyő aljától mérve.
  - `item_offset`: A tárgyak eltolása a tilemapon.
- **Visszatérési érték**: Nincs

### `render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height)`

- **Paraméterek**:
  - `renderer`: A rajzoló.
  - `item_tilemap`: A tárgyak textúrája.
  - `inventory_selection`: A kiválasztott tárgyak.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
- **Visszatérési érték**: Nincs

### `render_button(SDL_Renderer *renderer, ButtonType button)`

- **Paraméterek**:
  - `renderer`: A rajzoló.
  - `button`: A gomb típusa.
- **Visszatérési érték**: Nincs

### `is_button_clicked(ButtonType button, int x, int y)`

- **Paraméterek**:
  - `button`: A gomb típusa.
  - `x`: Az egér x koordinátája.
  - `y`: Az egér y koordinátája.
- **Visszatérési érték**: `bool` - Igaz, ha a gombra kattintottak, egyébként hamis.

### `is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot)`

- **Paraméterek**:
  - `x`: Az egér x koordinátája.
  - `y`: Az egér y koordinátája.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
  - `slot`: Az inventárió slot indexe.
- **Visszatérési érték**: `bool` - Igaz, ha az inventárió slotra kattintottak, egyébként hamis.

### `handle_aux_selection(int *selected_aux_item, int inventory_size, int offset)`

- **Paraméterek**:
  - `selected_aux_item`: Az aux karakterláda kiválasztott tárgyának indexe.
  - `inventory_size`: Az inventárió mérete.
  - `offset`: Az aux karakterláda slotjainak eltolása.
- **Visszatérési érték**: Nincs

### `render_debug_info(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height)`

- **Paraméterek**:
  - `renderer`: A rajzoló.
  - `character`: A karakter mutatója.
  - `inventory_selection`: Az inventárválasztó mutatója.
  - `grid`: A rács mutatója.
  - `foreground_grid`: Az előtér rács mutatója.
  - `crop_manager`: A növénykezelő mutatója.
  - `screen_width`: A képernyő szélessége.
  - `screen_height`: A képernyő magassága.
- **Visszatérési érték**: Nincs

- `window.h`: A programhoz szükséges típusdefiníciókat, konstansokat és függvényprototípusokat tartalmazza.

### Típusok

#### `ToolType`

Az eszköz típusokat definiálja.

```c
typedef enum
{
    TOOL_HOE,          // Kapa
    TOOL_WATERING_CAN, // Öntözőkanna
    TOOL_SICKLE,       // Sarló
} ToolType;
```

#### `Grid`

A rácsot reprezentáló struktúra.

```c
typedef struct
{
    int width;
    int height;
    int **physical_layer;
    int **optical_layer;
    bool **collision_layer;
} Grid;
```

#### `ForegroundGrid`

Az előtér rácsot reprezentáló struktúra.

```c
typedef struct
{
    int width;
    int height;
    int **background_layer;
    int **foreground_layer;
} ForegroundGrid;
```

#### `Direction`

Az animációk irányait definiálja.

```c
typedef enum
{
    DIRECTION_DOWN,  // Lefelé
    DIRECTION_UP,    // Felfelé
    DIRECTION_RIGHT, // Jobbra
    DIRECTION_LEFT   // Balra
} Direction;
```

#### `AnimationController`

Az animáció vezérlő struktúra.

```c
typedef struct
{
    int frame;
    int max_frames;
    int frame_delay;
    int frame_delay_counter;
    int frame_direction;
    Direction direction;
    bool is_walking;
} AnimationController;
```

#### `Character`

A karaktert reprezentáló struktúra.

```c
typedef struct
{
    int x;
    int y;
    int tile_x;
    int tile_y;
    int look_x;
    int look_y;
    int look_tile_x;
    int look_tile_y;
    ToolType equipped_tool;
    AnimationController anim_ctrl;
} Character;
```

#### `SeedType`

A mag típusokat definiálja.

```c
typedef enum
{
    SEED_PARSNIP,     // Parsnip
    SEED_CAULIFLOWER, // Karfiol
    SEED_COFFEE,      // Kávé
    SEED_GREEN_BEAN,  // Zöldbab
    SEED_HOPS,        // Komló
    SEED_POTATO,      // Burgonya
    SEED_STRAWBERRY,  // Eper
    SEED_MELON,       // Dinnye
    SEED_STARFRUIT,   // Csillaggyümölcs
} SeedType;
```

#### `SeedPrice`

A mag árakat definiálja.

```c
typedef enum
{
    SEED_PRICE_PARSNIP = 20,     // Parsnip ára
    SEED_PRICE_CAULIFLOWER = 80, // Karfiol ára
    SEED_PRICE_COFFEE = 15,      // Kávé ára
    SEED_PRICE_GREEN_BEAN = 60,  // Zöldbab ára
    SEED_PRICE_HOPS = 30,        // Komló ára
    SEED_PRICE_POTATO = 50,      // Burgonya ára
    SEED_PRICE_STRAWBERRY = 100, // Eper ára
    SEED_PRICE_MELON = 80,       // Dinnye ára
    SEED_PRICE_STARFRUIT = 200,  // Csillaggyümölcs ára
} SeedPrice;
```

#### `CropPrice`

A termény árakat definiálja.

```c
typedef enum
{
    CROP_PRICE_PARSNIP = 35,      // Parsnip ára
    CROP_PRICE_CAULIFLOWER = 175, // Karfiol ára
    CROP_PRICE_COFFEE = 20,       // Kávé ára
    CROP_PRICE_GREEN_BEAN = 40,   // Zöldbab ára
    CROP_PRICE_HOPS = 25,         // Komló ára
    CROP_PRICE_POTATO = 80,       // Burgonya ára
    CROP_PRICE_STRAWBERRY = 120,  // Eper ára
    CROP_PRICE_MELON = 250,       // Dinnye ára
    CROP_PRICE_STARFRUIT = 750,   // Csillaggyümölcs ára
} CropPrice;
```

#### `InventorySelection`

Az inventárválasztót reprezentáló struktúra.

```c
typedef struct
{
    int selected_main_item;
    int selected_aux_item;
    int selected_aux_inventory;
    int seed_counts[INVENTORY_SIZE];
    int harvest_counts[INVENTORY_SIZE];
    SeedType seed_types[INVENTORY_SIZE];
    int total_seeds;
    int money;
} InventorySelection;
```

#### `TileType`

A csempe típusokat definiálja.

```c
typedef enum
{
    TILE_EMPTY,               // Üres csempe
    TILE_HOE = 229,           // Kapált föld
    TILE_WATERED,             // Öntözött föld
    TILE_HOE_PLANTED,         // Kapált és ültetett föld
    TILE_HOE_PLANTED_WATERED, // Kapált, ültetett és öntözött föld
} TileType;
```

#### `CropType`

A növény típusokat definiálja.

```c
typedef enum
{
    CROP_PARSNIP,     // Paszternák
    CROP_CAULIFLOWER, // Karfiol
    CROP_COFFEE,      // Kávé
    CROP_GREEN_BEAN,  // Zöldbab
    CROP_HOPS,        // Komló
    CROP_POTATO,      // Burgonya
    CROP_STRAWBERRY,  // Eper
    CROP_MELON,       // Dinnye
    CROP_STARFRUIT,   // Csillaggyümölcs
} CropType;
```

#### `CropInfo`

A növény információkat tartalmazó struktúra.

```c
typedef struct
{
    CropType type;
    const char *name;
    int growth_stages;
    int texture_start;
} CropInfo;
```

#### `CropPhase`

A növény fázisokat definiálja.

```c
typedef enum
{
    CROP_PHASE_1,  // 1. fázis
    CROP_PHASE_2,  // 2. fázis
    CROP_PHASE_3,  // 3. fázis
    CROP_PHASE_4,  // 4. fázis
    CROP_PHASE_5,  // 5. fázis
    CROP_PHASE_6,  // 6. fázis
    CROP_PHASE_7,  // 7. fázis
    CROP_PHASE_MAX // Maximális növekedési fázis
} CropPhase;
```

#### `Crop`

A növényt reprezentáló struktúra.

```c
typedef struct
{
    int x;
    int y;
    CropType type;
    CropPhase growth_stage;
    int growth_time;
    int current_time;
} Crop;
```

#### `CropManager`

A növénykezelőt reprezentáló struktúra.

```c
typedef struct
{
    Crop *crops;
    int crop_count;
} CropManager;
```

#### `ButtonType`

A gomb típusokat definiálja.

```c
typedef enum
{
    BUTTON_ZOOM_IN,   // Nagyítás gomb
    BUTTON_ZOOM_OUT,  // Kicsinyítés gomb
    BUTTON_SAVE_GAME, // Játék mentése gomb
} ButtonType;
```
