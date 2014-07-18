struct Sprite;
struct TrigTable;
struct SDL_Surface;

inline int point_offscreen(struct SDL_Surface *fb, int x, int y);
inline void fill_buffer(struct SDL_Surface *f, struct SDL_Surface *g);
void fill_buffer_with_ignore(struct SDL_Surface *f, struct SDL_Surface *g, void* ignore);
inline void clear_buffer(struct SDL_Surface *f);
void draw_line (int x0, int y0, int x1, int y1, struct SDL_Surface *fb);
inline void draw_pixel (int x, int y, struct SDL_Surface *fb, void* value);
void draw_sprite(struct Sprite s, struct SDL_Surface *fb, int x, int y);
void draw_sprite_with_ignore(struct Sprite s, struct SDL_Surface *fb, int x, int y, void* ignore);
void rotate_sprite(struct Sprite in, struct Sprite rotBuf, int degrees);
