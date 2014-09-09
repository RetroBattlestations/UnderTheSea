In here you will find the BASIC program for a contest used on
http://reddit.com/r/RetroBattlestations. The program was intended to
be reusable and reasonably portable to many different BASIC
flavors. The "graphics" are character based and have some simple
sprites. It should be portable to any BASIC that has the ability to
position the cursor at a specific location.

This time around instead of just being a picture or an animation, it's
a game! You take control of a sea turtle in a sort of endless runner
type game. The turtle constantly swims from left to right and the A &
Z keys move the turtle up & down. You need to eat food (x/y characters
in most versions) for points. Running into enemies will cost you
air. You can refill your air by catching air bubbles, or you can
surface to refill completely. When you run out of air it's game over
and the turtle goes belly up.

=== Typing Tips ===
When typing the program in you can leave off any lines which begin
with REM, they are not needed for the program to run. On many
platforms you can leave out the whitespace between keywords and
operators. IBM BASIC is not one of those however.

Note: On the TRS-80 Color Computer and BBC Micro you need to include
      the spaces around any IF, AND, OR, or THEN statement.

If you make a mistake and don't want to retype the entire line, most
of the BASICs have a way to make corrections.

Apple II computers:
  On an Apple II+ use LIST <line number> to print the line with an
  error, then use ESC followed by A/B/C/D to move the cursor one step
  at a time. Position the cursor at the beginning of the line, then
  use the right arrow to move over the line and fix the error. Be sure
  to arrow all the way to the end of the entire line before you hit
  RETURN!

  On an enhanced Apple IIe, Apple IIc, or Apple IIgs you can also use
  ESC with the arrow keys. In 80 column mode (enter with PR#3) the
  cursor will change to a white block with a + in it, push ESC to drop
  out of movement mode.

BBC Micro:
  Use LIST <line number> to print the line with a mistake, then use
  the arrow keys to move up to the beginning of the line. Each press
  of the copy key will type in the character under the cursor. Make
  any necessary edits by just typing on the keyboard and using copy to
  avoid retyping everything.

Commodore 64, Plus/4, and 128:
  Like the others, use LIST <line number> to display the line with
  problems, then use the arrow keys to move up and make any
  corrections. By pressing shift-INST you can insert a blank character
  if you missed something. Unlike the Apple II you don't need to arrow
  to the end of the line before pushing RETURN.

TRS-80 model 100:
  This has to be the best built-in BASIC editor I've seen so far! Just
  type EDIT and the entire BASIC program will be loaded into the
  built-in word processor where you can make any changes you
  want. Press F8 to exit the editor and go back to BASIC.

IBM Cassette BASIC, Disk BASIC, Advanced BASIC, GW-BASIC:
  Type EDIT <line number> and it will print the line on the screen and
  put your cursor at the beginning of the line. Arrow left/right and
  you can use Insert & Delete to make corrections. Like Commodore
  BASIC, you don't need to arrow to the end of the line before pushing
  RETURN.
