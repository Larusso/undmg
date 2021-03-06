#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmg.h"
#include "hfslib.h"
#include "hfsplus.h"

char endianness;

void TestByteOrder() {
  short int word = 0x0001;
  char *byte = (char *)&word;
  endianness = byte[0] ? IS_LITTLE_ENDIAN : IS_BIG_ENDIAN;
}

int main(int argc, char *argv[]) {
  TestByteOrder();

  AbstractFile *in = createAbstractFileFromFile(stdin);
  AbstractFile *out = createAbstractFileFromFile(tmpfile());

  if (!out) {
    fprintf(stderr, "error: can't create tmp file\n");
    return 1;
  }

  int result = extractDmg(in, out, -1);
  if (!result) {
    fprintf(stderr, "error: the provided data was not a DMG file.\n");
  }

  Volume *volume = openVolume(IOFuncFromAbstractFile(out));

  HFSPlusCatalogRecord *record = getRecordFromPath("/", volume, NULL, NULL);

  extractAllInFolder(((HFSPlusCatalogFolder *)record)->folderID, volume);

  closeVolume(volume);

  free(out);

  return 0;
}
