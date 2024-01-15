#ifndef CROSS_SERIALIZERS_H
#define CROSS_SERIALIZERS_H

char *CrossSerializer_IntArr_Str(int *_arr, int _count);

char *CrossSerializer_Ref_Str(int i, int s, int l);
char *CrossSerializer_Inj_Str(int s, int l, char *_hex);

#endif
