#include "go.h"
#include "xfer.h"
#include <string.h>
#include <assert.h>

#include "init.proto.h"
#include "inithotband.proto.h"
#include "initdirs.proto.h"
#include "menu.proto.h"
#include "easymenu.proto.h"

void
checkgrowitemarray (short numitems, ControlHandle (***items)[])
{
  ControlHandle (**chh)[];
  short j;

  if (numitems % BANDARRAYSIZE == 0)
    {
/* FIXME - SetHandleSize not good enough for you? */
      chh = (ControlHandle (**)[])NewHandle (BANDARRAYSIZE * (numitems / BANDARRAYSIZE + 1)
					     * sizeof (ControlHandle));
      for (j = 0; j < numitems; j++)
	(**chh)[j] = (***items)[j];
      DisposHandle ((Handle) *items);
      *items = chh;
    }
}

void
tail (char *path, Str255 s)
{
  short i, j, len;

  len = strlen (path);
  i = len - 1;
/* FIXME - bad form not to reverse these checks.  The second loop
 *         here can sometimes check path[-1].
 * FIXME - What's wrong with strrchr?
 */
  while (path[i] == ':' && i > 0)
    i--;
  while (path[i] != ':' && i >= 0)
    i--;
  s[0] = len - i - 1;

/* FIXME - use memcpy or memmove!  This "i++ < len" is kinda scary. */
  for (j = 1; i++ < len; j++)
    s[j] = path[i];
}

void
getname (Str255 name, FILE * f)
{
  short i, c;

  name[0] = ' ';
  for (i = 1; (i < 255) && (name[i - 1] != ':') && (name[i - 1] != '\n'); i++)
    {
      c = getc (f);
      if (c == EOF)
	name[i] = '\n';
      else
	name[i] = c;
    }
  name[0] = i - 2;
}

OSErr
cd (short *cwd, Str255 name)
{
  short oldcwd;
  WDPBRec pb;
  OSErr e;

  pb.ioNamePtr = (StringPtr) name;
  pb.ioWDDirID = 0;
  pb.ioWDProcID = CREATOR;
  pb.ioVRefNum = *cwd;
  e = PBOpenWD (&pb, false);
  if (e == noErr)
    {
      oldcwd = *cwd;
      *cwd = pb.ioVRefNum;
      pb.ioVRefNum = oldcwd;
      e = PBCloseWD (&pb, false);
    }
  return e;
}

void
appenddir (char ***p, Str255 name)
{
  short l;
  OSErr e;

  l = strlen (**p);
  (**p)[l] = ':';
  e = PtrAndHand (name + 1, *p, name[0] + 1);
  (**p)[l + name[0] + 1] = 0;
}

OSErr
getonefileinfo (FILE * f, CInfoPBRec * cpb, char ***path, short *volume)
{
  ParamBlockRec pb;
  OSErr e;
  Str255 name;
  short cwd;

  pb.volumeParam.ioVolIndex = -1;
  pb.volumeParam.ioVRefNum = 0;
  getname (name, f);
  pb.volumeParam.ioNamePtr = name;
/* Add a colon to the end of name because that's what PBGetVInfo wants. */
/* FIXME - how does this add a colon? */
  name[0]++;
  e = PBGetVInfo (&pb, false);
  *volume = pb.volumeParam.ioVRefNum;
  cwd = pb.volumeParam.ioVRefNum;
  *path = (char **) NewHandle (name[0] + 1);
  PtoCstr (name);
  strcpy (**path, (char *) name);
  getname (name, f);
  appenddir (path, name);

  while (name[name[0] + 1] == ':')
    {
      /* FIXME - ORing together error codes is punishable by death. */
      e |= cd (&cwd, name);
      getname (name, f);
      appenddir (path, name);
    }

  cpb->hFileInfo.ioVRefNum = cwd;
  cpb->hFileInfo.ioDirID = 0;
  cpb->hFileInfo.ioFDirIndex = 0;
  cpb->hFileInfo.ioNamePtr = name;
  /* FIXME - ORing together error codes is punishable by death. */
  e |= PBGetCatInfo (cpb, false);
  return e;
}

void
setupmenus (void)
{
  Handle h;
  MenuHandle mh;

  h = GetNewMBar (GOMBARID);
  SetMenuBar (h);
  DrawMenuBar ();

  mh = GetMHandle (FIRSTMENU);
  AddResMenu (mh, 'DRVR');
  menuchoices (false);
  showviewmenu (false);
}

void
setdefaulteditor (void)
{
  Str255 s, name;
  StringHandle sh;
  short i, done;
  short cwd;
  CInfoPBRec cpb;
  OSErr e;
  HParamBlockRec pb;

  sh = GetString (DEFAULTEDITORSTRINGID);
  HLock ((Handle) sh);
  mystr255copy (name, *sh);
  HUnlock ((Handle) sh);

#if defined(OK_LETS_DISPOSE_RESOURCES_JUST_TO_BREAK_THINGS)
  DisposHandle ((Handle) sh);
#endif

  mystr255copy (s, name);
  pb.volumeParam.ioNamePtr = s;
  if (name[1] == ':')
    {
      pb.volumeParam.ioVolIndex = 0;
      pb.volumeParam.ioVRefNum = BootDrive;
      i = 1;
    }
  else
    {
      mystr255copy (s, name);
      for (i = 1; i < s[0] && s[i] != ':'; i++)
	;
      s[0] = i;
      pb.volumeParam.ioVolIndex = -1;
      pb.volumeParam.ioVRefNum = 0;
      /* FIXME - this is a pascal string, so there's no trailing '\0'
       * guaranteed to stop this loop.
       */
      while (s[i] == ':')
	i++;
      i--;
    }
  e = PBHGetVInfo (&pb, false);
  done = (e != noErr);
  cwd = pb.volumeParam.ioVRefNum;
  mystr255copy (s, name);
  s[i] = name[0] - i;
  mystr255copy (name, (StringPtr) &s[i]);

  while (!done)
    {
      mystr255copy (s, name);
      for (i = 1; i < s[0] && s[i] != ':'; i++)
	;
      s[0] = i;
      if (s[i] == ':')
	{
	  s[0]--;
	  done = cd (&cwd, s);
	  s[i] = name[0] - i;
	  mystr255copy (name, (StringPtr) &s[i]);
	}
      else
	done = true;
    }

  cpb.hFileInfo.ioVRefNum = cwd;
  cpb.hFileInfo.ioDirID = 0;
  cpb.hFileInfo.ioFDirIndex = 0;
  cpb.hFileInfo.ioNamePtr = name;
  e = PBGetCatInfo (&cpb, false);

  if (e == noErr)
    {
      sigowners[TEXTEDITORPOS] = (applist **) NewHandle (sizeof (applist));
      (*sigowners[TEXTEDITORPOS])->next = 0;
      (*sigowners[TEXTEDITORPOS])->parid = cpb.hFileInfo.ioFlParID;
      (*sigowners[TEXTEDITORPOS])->vrefnum = pb.volumeParam.ioVRefNum;
      mystr255copy ((*sigowners[TEXTEDITORPOS])->name, name);
    }
}

void
init (void)
{
  FILE *f;

#if 0
/* todo: find out how this is done these days */
  mystr255copy (FinderName, GONAME);
#endif /* 0 */

  InitGraf ((Ptr) & qd.thePort);
  InitFonts ();
  InitWindows ();
  InitMenus ();
  FlushEvents (everyEvent, 0);
  TEInit ();
  InitDialogs ((ProcPtr) nothing);	/* todo: write a resumeproc */

  g_selection = (ControlHandle (**)[])NewHandle (sizeof (ControlHandle) * 1);
  (**g_selection)[0] = (ControlHandle) 0;
  g_movecursor = GetCCursor (MOVECURSORID);
  g_copycursor = GetCCursor (COPYCURSORID);

/* Arguments Schmarguments */
  *(short *) (*AppParmHandle + 2) = 0;

  setdefaulteditor ();
  verify_flags = VERIFY_OVERWRITE_FILE | VERIFY_OVERWRITE_FOLDER |
    VERIFY_DELETE_FILE | VERIFY_DELETE_FOLDER;

  f = fopen (GOSAVEFILE, "r");
  inithotband (f);
  if (f != 0)
    {
      initopendirs (f);
    }
  else
    {
      f = fopen (GOSAVEFILE, "w");
      /* FIXME - why bother? */
      assert (f);
    }
  /* FIXME - if that assert goes away because of NDEBUG, we can end
   * up doing an fclose on NULL.  Very, very bad.
   */
  fclose (f);

  setupmenus ();
  InitCursor ();
}
