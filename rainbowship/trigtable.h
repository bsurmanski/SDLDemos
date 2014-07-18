typedef struct TrigTable TrigTable;

TrigTable *init_table();
float lookup_sin(TrigTable *t, float val);
float lookup_cos(TrigTable *t, float val);
float lookup_tan(TrigTable *t, float val);
