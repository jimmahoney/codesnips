#!/usr/bin/perl -w
##############################
#
# TicTacToe.pl
#   is a example of a text based two player game,
#   including documentation, tests, user input/ouput,
#   and recursive search for the best computer move.
# 
# Contents
#   * header comments (you're reading 'em.)
#   * description of board data structure
#   * brief summary of all subroutines 
#   * global variables
#   * main program
#   * subroutines 1 : general TicTacToe stuff
#   * theory behind two player game computer searches
#   * subroutines 2 : computer search and analysis
#   * subroutines 3 : testing and debugging
#   * POD (Plain Ol' Docs) overview
#
# Development Comments
#   The first part of this that I wrote was the testing subroutines;
#   then I added functionality slowly, adding tests as I went.
#   The rough order what's in run_tests: 
#     * setting up and printing the board,
#     * getting input from the user, 
#     * looking for a win
#     * generating random moves, 
#     * playing a user/user or user/random games,
#     * computer searches for best move,
#     * statistics of searches.
#     * memo stuff - remembering previous search results.
#
#   All together I probably spent about 20 hours on this
#   over a long weekend.  Although intended as in example
#   of what could be done with the project assigned
#   in my intro programming class, the scale of what I 
#   ended up with would be more a term paper for an intermediate
#   programming class, not a two week exercise for an intro class.
#
# So ya wanna play a game?
#
# copyright Jim Mahoney (mahoney@marlboro.edu), October 2003
#
# This is free software; you can redistribute it and/or modify
# it under the same terms as Perl itself. 
##############################
use strict;
my $VERSION = "0.31 10/22/03";

# ==== description of board data  ================================
#
# The board is a simple array.
# Once the game is in progress, it'll look something like
#
#   ( 'X', ' ', ' ',
#     ' ', 'O', ' ',
#     ' ', 'X', ' ' )
#
# The user types a letter ('a' through 'i') to move at these 0..8 locations
#     @board indeces:  0 1 2      user types:   a b c
#                      3 4 5                    d e f
#                      6 7 8                    g h i 
# The globals @letters and %numbers covert between the
# numeric 0..8 board coordinates and the user input 'a'..'i' letters.
# 
# The global variables that store the current game situation
# are @board and @moves, both initilized by init_game().
# The possible values of the @board[0..8] are 'X', 'O', or ' '.
# X always plays first, so if the length of @moves is 0 or 
# any other even number, then it's X's turn to move.
#

# === list of subroutines ==============================
#
#              init_game();
#  $text     = board_as_text();
#  $boolean  = is_legal_move($move);         # $move is 0..8
#  $XorO     = whose_move();                 # 'X' or 'O' from @board, @moves.
#  $XorO     = find_winner();                # or return '' if no one has won.
#  ($p1,$p2) = choose_players();             # ask for 'user', 'random', ...
#  $move     = get_move($player);
#  @list     = get_legal_moves();
#  $move     = get_random_move();            # gives only legal moves
#  $int      = random($n);                   # 0..($n-1)
#              check_for_quit($input);       # see if user wants to quit
#  $input    = get_user_input();             # and check for 'q', 'quit'
#  $move     = get_user_move();              # asks for letter to specify move
#              do_move($move);          
#              undo_move();
#              play_game($player1, $player2);
#
#  $value    = evaluate();                   # >0,0,<0,undef values for @board
#              init_statistics();            # turn on and init stats globals
#              compile_statistic();          # append @board to statistics
#  $string   = statistics_summary();         # return time elapsed and stats
#  $string   = elapsed_time();               # since init_statistics, in secs.
#  $move     = get_minmax_move();            # calls minmax()
#  ($v,$m)   = minmax();                     # recursive (value,move) search.
#  $move     = get_alphabeta_move();         # calls alphabeta()
#  ($v,$m)   = alphabeta();                  # ditto, but with pruning.
#  $move     = get_computer_move();          # random for 1st, then alphabeta
#  ($v,$m)   = memoalphabeta();              # alphabeta with memory
#  $string   = board_as_key();               # unique string for @board
#              memo_store($val,$best,$key)   # save results of a search
#  $boolean  = memo_exists($key);            # searched this @board before?
# ($v,$m,..) = memo_get($key);               # retrieve previous search.
#              all_moves();                  # do memo on all moves recursively
#              memo_analyze();               # search all moves; print analysis
#
#              ok( $boolean, $comment);      # prints "1 ok comment"
#              run_tests();

# ==== globals =====================================================
# @board and @moves hold the current position;
# they're given "empty" values in init_game();
my @board;                            # $board[0..8] are 'X', 'O', or ' '.
my @moves;                            # X always goes first.
my $error    = -1;                    # constant that means 'illegal move'.
my @letters  = 'a'..'i';              # Convert from 0..8 to 'a'..'i'.
my %numbers = ( a=>0, b=>1, c=>2,     # Convert from 'a'..'i' to 0..8.
                d=>3, e=>4, f=>5,
                g=>6, h=>7, i=>8,
              );
my @winning_lines = ( [ 0, 1, 2 ],    # Three horizontal ways to win,
                      [ 3, 4, 5 ],    # 
                      [ 6, 7, 8 ],
                      [ 0, 3, 6 ],    # three vertical, and
                      [ 1, 4, 7 ],
                      [ 2, 5, 8 ],   
                      [ 0, 4, 8 ],    # two diagonal.
                      [ 2, 4, 6 ],
                    );
my $test_number  = 0;                 # Used by ok() and run_tests().
my $failed_tests = 0;                 # Ditto.
my @legal_players = ('user',
                     'computer',      # The X and O sides can be any 
                     'random',        # one of these.
                     'minmax',
                     'alphabeta',
                    );
my $infinity = 100;                   # larger than any evaluation() result. 
# These next four are initialized in init_statistics().
my $positions_examined;               # during search for best move.
my @positions_at_depth;               # details of search for best move.
my $start_depth;                      # number of previous moves before search
my $start_time;
# Stored results of computer search; see memoalphabeta()
# The form of this hash are entries like ( "X XO O   " => [undef, 1, 4, 'X'] )
# where the hash key is the output of board_as_key(), 
# and the hash value is an array reference containing four scalars, namely
# board_value, best_move, number_of_moves, and whose_turn.
my %memo_data = ();
my $all_counter;       # global counter for times through all_moves

# ==== main  ========================================================

## During program development I tend to uncomment the next two lines.
# run_tests();
# exit;

print "\n -- Tic Tac Toe --    version $VERSION \n";
while (1){

  print "
 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store best moves; summarize results
   quit     - quit
 What would you like to do ? ";

  my $input = get_user_input();   # check for 'q' is built in.

  if    ($input eq 'p'    or $input eq 'play'){
    play_game( choose_players() );
  }
  elsif ($input eq 't'    or $input eq 'tests'){
    run_tests();
  }
  elsif ($input eq 'a'    or $input eq 'analyze'){
    memo_analyze();
  }
  else {
    print " Sorry, but I don't know how to do '$input'.\n";
  }

}

# ==== general subroutines =============================================

# Set globals @board and @moves to initial values.
# Usage: init_game();
sub init_game {
  @board = (' ')x9;
  @moves = ();
}

# Return text string for current board position.
# Usage: print board_as_text();
sub board_as_text {
  return "
    $board[0] | $board[1] | $board[2]        a | b | c 
   ---" .   "+---" .    "+---" .    "      ---+---+---
    $board[3] | $board[4] | $board[5]        d | e | f 
   ---" .   "+---" .    "+---" .    "      ---+---+---
    $board[6] | $board[7] | $board[8]        g | h | i 
";
}

# See if given move is legal on global @board,
# i.e. whether that space is empty.
# Usage: if (is_legal($move){...}
sub is_legal_move {
  my ($move) = @_;
  return $board[$move] eq ' ';
}

# Return 'X' or 'O' depending on which is the next move
# on the global @board given the move history stored in @moves.
# X has the fist move goes first.
sub whose_move {
  if ( 0 == scalar(@moves) % 2 ) { #  if number of previous moves is even
    return 'X';                    #  then its X's turn
  }
  else {
    return 'O';                    #  else its O's turn.
  }
}

# Return 'X' or 'O' if someone on the global @board has won, 
# otherwise return false.  
# Uses the global @winning_lines to define what winning means.
# Usage: my $winner = find_winner(); if ($winner){ print "$winner has won\n;}
sub find_winner {
  foreach my $d (@winning_lines){
    my $XorO = $board[$d->[0]];
    next if $XorO eq ' ';            # win can't have an empty space
    return $XorO if ($XorO eq $board[$d->[1]] and $XorO eq $board[$d->[2]]);
  }
  return '';  # no winner found.
}

# Ask the user to pick the X and O players 
# (i.e. algorithms for choosing the next move)
# (user, computer, ...) and then start the game
# with those players.  Returns a list of two strings, eg ('user', 'computer').
# Usage: ($Xplayer, $Yplayer) = choose_players();
sub choose_players {
  my @result = ();
  my $default = 'computer';
  print " 
 The first player is X; the second is O.
 Each can be one any of (@legal_players).
 Type 'help' or 'h' for an explanation of the choices.\n";
  for my $which ('X', 'O'){
    my $choice = '';
    while (not $choice){
      print " Player $which is ? ";
      my $input = get_user_input();
      if ($input eq 'h' or $input eq 'help'){
        print "
  The methods of choosing the next move are
   user       - you input moves from the keyboard
   computer   - random first X move, then memoalphabeta search  (i.e. smart)
   random     - randomly chosen legal move                      (i.e. stupid)
   minmax     - min/max recursive search with positions and time summary
   alphabeta  - alpha/beta pruned search with positions and time summary \n\n";
     }
      if ($input eq ''){
        $choice = $default if $input eq '';
        print " Choosing default '$default' for $which. \n";
      }
      for my $legal (@legal_players){
        # This is a legal choice if its in the list, 
        # or if it matches the first letter of something in the list.
        if ($input eq $legal or $input eq substr($legal,0,1)){
          $choice = $legal;
        }
      }
      if (not $choice and not ($input eq 'h' or $input eq 'help')){
        print " Sorry, I don't understand '$input'.\n" unless $choice;
      }
      elsif ($choice and length($input)==1){
        print " OK, $which is '$choice'.\n";
      }
    }
    push @result, $choice;
  }
  return @result;
}

# Return a legal move (integer 0..8 when the board is empty)
# from one of the various kinds of players (user, computer, ...)
# given the situation defined by the globals @board and @moves.
# Return $error if there are no legal moves.
# Usage: $move = get_move($player);
sub get_move {
  my ($player) = @_;
  my $mark = whose_move();
  if ($player eq 'user'){
    return get_user_move();
  }
  elsif ($player eq 'computer'){
    my $move   = get_computer_move();
    print "\n Computer playing '$mark' chooses '$letters[$move]'.\n";
    return $move;
  }
  elsif ($player eq 'random'){
    my $move   = get_random_move();
    print "\n Random computer playing '$mark' chooses '$letters[$move]'.\n";
    return $move;
  }
  elsif ($player eq 'minmax'){
    my $move = get_minmax_move();
    print "\n Minmax computer playing '$mark' chooses '$letters[$move]'.\n";
    return $move;
  }
  elsif ($player eq 'alphabeta'){
    my $move = get_alphabeta_move();
    print "\n AlphaBeta computer playing '$mark' chooses '$letters[$move]'.\n";
    return $move;
  }
  else {
    print "\n Oops - I don't know how to make a '$player' move.\n";
    return $error;
  }
}

# Return a list of the current legal moves on @board,
# i.e. locations 0..8 where the board is ' '.
# Usage: @legal_moves = get_legal_moves();
sub get_legal_moves {
  my @legal;
  for my $move (0..8){
    push(@legal, $move) if is_legal_move($move);
  }
  return @legal;
}

# Return a random legal move on the current global @board.
# Usage: $move = get_random_move();
sub get_random_move {
  my ($board) = @_;
  my @legal = get_legal_moves();
  return $error unless @legal;                 # return error - no legal moves
  return $legal[ random(scalar(@legal)) ];     # return random move in @legal.
}

# Return random integer from 0 to $n-1
# usage: $i = random($n)
sub random {
  my ($n) = @_;
  return int($n*rand());
}

# If input is 'q' or 'quit', 
# ask for confirmation and exit program if received.
# Usage: $input=<>; check_for_quit($input);
sub check_for_quit {
  my ($input) = @_;
  if ($input eq 'q' or $input eq 'quit'){
    print " Are you sure you want to quit the program? [y] ";
    my $confirm = <>;
    chomp($confirm);
    if ($confirm eq 'y' or $confirm eq 'yes' or $confirm eq ''){
      print "\n Bye.\n\n";
      exit;
    }
  }
}

# Get input character or word from the keyboard.
# Always check for 'q' or 'quit', and if confirmed exit the program.
# Usage: my $input = get_user_input();
sub get_user_input {
  my $input = <>;
  chomp($input);
  check_for_quit($input);
  return $input;
}

# Ask the user for a legal move.
# Usage: $move = get_user_move(); 
sub get_user_move {
  my $X_or_O = whose_move();
  while (1){            # Loop until a legal move is entered.
    print " Type 'a'..'i' to play an '$X_or_O', or type 'q' to quit : ";
    my $letter = get_user_input();
    if ( length($letter) != 1 ){
      print " Oops - please type a single letter.\n";
    }
    elsif ( $letter lt 'a' or $letter gt 'i'){
      print " Oops - '$letter' isn't in (a,b,c,d,e,f,g,h,i).\n";
    }
    elsif ( $board[ $numbers{$letter} ] ne ' ' ){
      print " Oops - there's already something at '$letter'.\n";
    }
    else {                        # This letter looks OK;
      return $numbers{$letter};   # return corresponding move number.
    }
  }
}

# Apply a move to the global @board and remember it in @moves.
# No tests are done here for the legality of the move.
# An 'X' or 'O' is placed depending on the return value of whose_move().
# whether to put an X or O at the $move location.
# Usage: do_move($move);
sub do_move {
  my ($move) = (@_);
  my $mark = whose_move();
  $board[$move] = $mark;
  push @moves, $move;
}

# Undo the last move by removing it from the @moves history 
# and putting ' ' on that location on the global @board.
# Usage: undo_move()
sub undo_move {
  my $move = pop @moves;
  $board[$move] = ' ';
}

# Play a TicTacToe game.  X always plays first; O goes second.
# The possible kinds of players (user, computer, ...) are 
# defined in the global @legal_players.
# Usage: play_game($Xplayer, $Yplayer); 
sub play_game {
  my @players = @_;
  init_game();
  while (1){
    print board_as_text() . "\n";
    my $which  = scalar(@moves) % 2;   # 0 if X's turn (move 0,2,4,...) or 1.
    my $player = $players[ $which ];   # One of @legal_players = ('user',...)
    my $move   = get_move($player);    # Ask user, pick random, or do search.
    do_move($move);                    # Apply this move to the @board global.
    my $winner = find_winner();
    if ($winner){
      print board_as_text() . "\n";
      print "\n   ==> $winner wins! <== \n";
      return;
    }
    elsif (scalar(@moves)==9){
      print board_as_text() . "\n";
      print "\n   ==> Draw! <==\n";
      return;
    }
  }
}

# ==== theory of two player game searching ======================
#
# Think of the board positions as a tree, with the current position
# as the root of the tree, and the branches as possible moves.
# 
#        current
#        |
#     +-------+-------+
#     A       B       C               positions after one move
#     |       |       |
#    +--+    +--+    +--+--+ 
#    D  E    F  G    H  I  J          positions after two moves
# 
#
# To find the best computer move we need to search such a tree,
# and typically the easiest way to do this is with recursion.
#
# The first decisision we need to make is how far to go.
# Typically we choose some of the nodes as "terminal" points, and
# we must have a way to assign a "value" for those positions without
# doing further searching.  In a game like chess the tree is way
# to big to search completely, so we do something like pick a depth,
# say 6 moves ahead, and stop at those positions.
#
# For this TicTacToe we can just search all the way down, stopping
# only if a position is a win, a loss, or a draw; those are the only
# terminal nodes in this case.
#
# With values for the bottom terminal nodes in hand, we can move upward
# in the tree, assigning values for the higher nodes based on whose
# move it is and which branch they would choose.  Since alternating
# players are striving for opposite goals, this is a "MinMax" technique.
# There are several ways to go about this; what follows are templates
# of what the code might look like.
#
# Here are three templates for Min/Max, MinMax, AlphaBeta,
# three successively more sophisticated searching algorithms.
# All three of these examples are adapted from a description
# of a chess algorithm by Bruce Moreland (the See Also section
# below gives the reference).  All of these return the value of
# the current position, which lets you figure out the best move.
#
# # -------------------------------------------------------------------
# # This one alternates finding the best (Max) or wost (Min) position
# # using an evalution subroutine, EvaluateForX, which 
# # returns positive numbers if the position is good for X, 
# # and negative numbers if the position is good for Y.
# # You'd invoke this with something like
# #
# #  if (SideToMove() == 'X'){      # the "maximizing" player.
# #      return Max($search_depth);}
# #  else {                         # the "minimizing" player.
# #      return Min($search_depth);}
#
# sub Max {
#   my ($depth) = @_;
#   my $best = -$INFINITY;
#     if ($depth <= 0){ return EvaluateForX(); }
#     GenerateLegalMoves();
#     while (MovesLeft()) {
#         MakeNextMove();
#         my $value = Min($depth - 1);
#         UnmakeMove();
#         if ($value > $best){ 
#             $best = $value;
#         }
#     }
#     return $best;
# }
# sub Min {
#   my ($depth) = @_;
#   my $best = $INFINITY;      # <-- Note that this is different than in "Max".
#     if ($depth <= 0){ return EvaluateForX(); }
#     GenerateLegalMoves();
#     while (MovesLeft()) {
#         MakeNextMove();
#         my $value = Max($depth - 1);
#         UnmakeMove();
#         if ($value < $best){ # <-- Note that this is different than in "Max".
#             $best = $value;
#         }
#     }
#     return $best;
# }
#
# --------------------------------------------------------------
# # Next, MinMax does exactly the same search, but combines both
# # the Min and Max routines into one.  (Tricky, eh?)
# # This time the Evaluate function must return positive for positions 
# # which are good for the player whose move it is.
# # On the first call, depth is how far down you'd like to search, so
# # you invoke this with something like
# # $value = MinMax($search_depth);
# sub MinMax {
#   my ($depth) = @_;
#   my $best = -$INFINITY;
#   if ($depth == 0) { return Evaluate(); } # $depth==0 implies a terminal node
#   GenerateLegalMoves();
#   while (MovesLeft()) {
#       MakeNextMove();
#       my $value = -MinMax($depth - 1);    # Note the minus sign here.
#       UnmakeMove();        
#       if ($value > $best) { $best = $value; }
#   }
#   return $best;
# }
#

# ------------------------------------------------------
# # Finally, here's an improvement on MinMax, which abandons the
# # search if we've already seen something better elsewhere, thus
# # "pruning" the search tree and giving better performance.
# # Again, the Evaluate function returns positive/negative if 
# # the position is good/bad for the player whose move it is.
# # $alpha is the best node value so far for the person whose move it is; 
# # $beta is the best value for the opponent seen so far; keeping track
# # of that allows you to stop looking at part of the search tree once
# # you see that your opponent won't let that happen.
# # On the first call, you should start the recursive tree by asking for
# # $value = AlphaBeta($search_depth, -$infinity, $infinity);
# sub AlphaBeta {
#   my ($depth, $alpha, $beta) = @_;
#   if ($depth == 0) {return Evaluate();} # If terminal node, return its value.
#   GenerateLegalMoves();                 # Note that alpha/beta does better
#   while (MovesLeft()) {                 # if moves are ordered good to bad.
#      MakeNextMove();
#      my $value = -AlphaBeta($depth - 1, -beta, -alpha);
#      UnmakeMove();        
#      if ($value >= $beta) { return $beta;    }
#      if ($value > $alpha) { $alpha = $value; }
#   }
#   return $alpha;
# }
#
#

#
# === code for min/max search and alpha/beta pruning =================
#
# Several pieces have been added to the discussion above, including
#  * the return of the best move as well as the board value,
#  * a version that stores the best moves rather than repeat the search, and
#  * various ways to summarize the results.
# Now we're gonna have some fun...

# Return the value of the current board position 
# given in the global @board and @moves, defined here for TicTacToe as
#   positive        if the player whose turn it is has won,
#   negative        if the player whose turn it is has lost,
#   0               if the game is a draw,
#   undef           if the game is still undecided (we'll look deeper)
# The first time I wrote this, I had winning and losing as +1 and -1.
# That worked, but the algorithm wouldn't always pick the immediate
# winning move, because it found a win later on. To fix that, 
# I'm setting the value to be larger if the win happens in fewer moves.
# Note that the globals ($infinity,-$infinity) must be (larger,smaller) 
# than any possible return value.
# Usage: $value = evaluate();
sub evaluate {
  my $winner = find_winner();            # 'X', 'O', or ''
  my $value  = 10 - scalar(@moves);      # a positive number, larger earlier
  if ( whose_move() eq $winner ){
    return $value;                       # I win;  return positive
  }
  elsif ( $winner ){                     # I lose; return negative
    return -$value;
  }
  elsif ( scalar(@moves) == 9 ) {
    return 0;                            # Draw;   return 0
  }
  else {
    return undef;                        # Not a terminal node.
  }
}

# Set globals used for analyzing positions to initial value
# Usage: init_statistics();
sub init_statistics {
  $positions_examined = 0;
  @positions_at_depth = ();
  $start_depth = scalar(@moves);
  $start_time  = time();       # in seconds since the epoch - 1960-ish.
}

# Add current global @board, @moves position to the search statistics.
# Usage: compile_statistic();
sub compile_statistic {
  $positions_examined++;
  $positions_at_depth[ scalar(@moves) - $start_depth ]++;
}

# Return string containing statistics and elapsed time.
# Usage: print statistics_summary();
sub statistics_summary {
  return "     $positions_examined total positions examined in "
       . elapsed_time() . ".\n"
       . "     Breakdown by depth is (@positions_at_depth).\n";
}

# Return rough elapsed time since init_statistics called
# Usage: print elapsed_time();
sub elapsed_time {
  my $seconds = time() - $start_time;
  if ($seconds == 0){
    return "less than a second";
  }
  elsif ($seconds ==1){
    return "about a second";
  }
  else {
    return "$seconds seconds";
  }
}

# Return the results from a full min/max (implemented as negamax) search,
# and print some performance diagnostics.  
# For the first move this will search 549946 positions in something
# under two minutes (500 MHz Powerbook, including statistics; its about
# a minute and half without the statistics) and then decide to 
# move in the top left corner.  Go figure.  
# Usage: $move = get_minmax_move.
sub get_minmax_move {
  ## If this is the first move of the game, just return a random move.
  # return get_random_move() unless @moves;
  # Otherwise, do a minmax() search.
  init_statistics();
  print "     Starting minmax search ... \n";
  my ($value, $bestmove) = minmax();
  print "     ... done.\n";
  print statistics_summary();
  return $bestmove;
}

# Return the value and best move from the current position 
# with a full min/max recursive search.  
# All moves are done (and undone) on the global @board and @moves.
# This algorithm doesn't remember the results of previous searches.
# To call this and get just a value, use ($value) = minmax();
# Usage: ($value, $bestmove) = minmax();
sub minmax {
  # my $debug = ( scalar(@moves) == 4 ); # print debug info if 4 moves deep
  compile_statistic();
  my $value = evaluate();
  # print "  nega: value = 'undef'\n" if $debug and ! defined $value;
  # print "  nega: defined value = '$value'\n" if $debug and defined $value;
  return $value if defined $value;     # undefined means no win/lose/draw yet.
  my @legal_moves = get_legal_moves();
  # print "  nega: legal moves = (@legal_moves)\n" if $print;
  my $best = -$infinity;
  my $best_move = $error;
  for my $move (@legal_moves){
    do_move($move);
    ($value) = minmax();    # Recursive search for value of next position.
    $value = -$value;        # That value is from other players point of view.
    undo_move();
    ($best, $best_move) = ($value, $move) if $value > $best;
  }
  return ($best, $best_move);
}

# Return a computer move and print some performance diagnostics
# for an alphabeta search (even for 1st move, unlike 'computer' player).
# For the first move, the tree is pruned from 549946 positions 
# down to 20866, which reduces the time required from about 100 seconds 
# to about 4 seconds (including the statistics) on my 500MHz Powerbook.
# Usage: $move = get_alphabeta_move();
sub get_alphabeta_move {
  init_statistics();
  print "     Starting alphabeta search ... \n";
  my ($value, $bestmove) = alphabeta( -$infinity, $infinity);
  print "     ... done.\n";
  print statistics_summary();
  return $bestmove;
}

# Return the value and best move from the current position 
# using a pruned alpha/beta search.  
# All moves are done (and undone) on the global @board and @moves.
# This doesn't remember the results of previous searches.
# Usage: ($value, $bestmove) = alphabeta();
# To call this and get just a value, use ($value) = alphabeta();
sub alphabeta {
  my ($alpha, $beta) = @_;
  compile_statistic();
  my $value = evaluate();
  return $value if defined $value;     # undefined means no win/lose/draw yet.
  my @legal_moves = get_legal_moves();
  my $best_move = $error;
  for my $move (@legal_moves){
    do_move($move);
    ($value) = alphabeta(-$beta, -$alpha);    # Recursive search
    $value = -$value;                         # other players perspective
    undo_move();
    if ($value >= $beta){
      return ($beta, $move);
    }
    if ($value > $alpha){
      ($alpha, $best_move) = ($value, $move);
    }
  }
  return ($alpha, $best_move);
}

# Return a reasonable computer move using the fastest
# of the search methods, namely memoalphabeta.
# Since search results are saved, things will run faster
# after the first game or after memo_analyze.
# Usage: $move = get_computer_move();
sub get_computer_move {
  return get_random_move() unless @moves;            # Random first X move.
  my ($value, $bestmove) = memoalphabeta( -$infinity, $infinity);
  return $bestmove;
}

# Return the value and best move from the current position 
# using a memoized alpha/beta search; that is, remembering each
# result and simply returning it if it's already known.
# All moves are done (and undone) on the global @board and @moves, 
# and remembered in the global %memo_data hash.
# To call this and get just a value, use ($value) = memoalphabeta();
# Usage: ($value, $bestmove) = memoalphabeta();
sub memoalphabeta {
  my ($alpha, $beta) = @_;
  my $boardkey = board_as_key();       # unique string identifying board
  return memo_get($boardkey) if memo_exists($boardkey);
  my $value = evaluate();
  if (defined $value){
    memo_store($value,$error,$boardkey);
    return ($value, $error);
  }
  my @legal_moves = get_legal_moves();
  my $best_move = $error;
  for my $move (@legal_moves){
    do_move($move);
    ($value) = memoalphabeta(-$beta, -$alpha);  # Recursive search
    $value = -$value;                           # other players perspective
    undo_move();
    if ($value >= $beta){
      # memo_store($beta, $move, $boardkey); # This abandons search;
      return ($beta, $move);                    # I don't this is a best move.
    }
    if ($value > $alpha){
      ($alpha, $best_move) = ($value, $move);
    }
  }
  memo_store($alpha, $best_move, $boardkey) unless $best_move == $error;
  return ($alpha, $best_move);
}

# Return a unique key that identifies the current board position.
# While this could be encoded as a number (3 symbols in 9 positions
# means 0..3**9-1, or 0..19682), here I'm doing a simpler thing and
# just returning a string by concatenating the ' ', '0', 'X' marks.
# Usage: $key = board_as_key();
sub board_as_key {
  return join('',@board);
}

# Save results of a search for later retrieval.
# Uses the global %memoData.
# Usage: memo_store($value, $bestmove, $boardkey);
sub memo_store {
  my ($value, $bestmove, $boardkey) = @_;
  $memo_data{$boardkey} = [ $value, $bestmove, scalar(@moves), whose_move()];
}

# Has this board position been searched before?
# Uses the global %memo_data.
# Usage: if (memo_seeen($boardkey)) {...}
sub memo_exists {
  my ($boardkey) = @_;
  return exists $memo_data{$boardkey};
}

# Return the saved results of a previous search.
# Uses the global %memo_data;
# Usage: ($value, $bestmove, $turns, $whosemove) = memo_get($boardkey)
sub memo_get {
  my ($boardkey) = @_;
  return @{$memo_data{$boardkey}};
}

# Find value and best move for all positions below this one recursively,
# whether or not they're positions that would be reached by searching
# with best play from root. 
# 
# Usage: init_game(); all_moves();
sub all_moves {
  $all_counter++;
  # The memoalphabeta routine has the side effect of 
  # storing each unique position, its value, and the best move in
  # that position in the memo_ globals.
  memoalphabeta(-$infinity,$infinity);
  foreach my $move (get_legal_moves()){
    do_move($move);
    all_moves();
    undo_move($move);
  }
}

# Generate the tree of all possible moves, remember the result in
# the global %memo_data, and print a summary.
# Usage: memo_analyze()
sub memo_analyze {
  init_game();              # Intialize @board, @moves
  init_statistics();        # starts timer; don't need other stats here.
  %memo_data = ();        # Forget any old stored results.
  print " Analyzing all possible board positions";
  print " (this takes a few minutes ) ... \n";
  $all_counter = 0;        # global incremented in all_moves()
  all_moves();
  print " ... done; considered $all_counter positions in " 
        . elapsed_time() . ".\n";
  print "    Number of distinct positions found = " . 
    scalar(keys %memo_data) . ".\n";
  print " Computer player should now respond with good moves instantly.\n";
  # These are for each "depth", i.e. each number of possible moves 0 through 9
  my @counts = (0)x10;  #  = (0,0,0,0,0,0,0,0,0,0) 
  my @X_wins = (0)x10;
  my @O_wins = (0)x10;
  foreach my $key (keys %memo_data){
    my ($value, $bestmove, $depth, $whoseturn) = memo_get($key);
    $counts[$depth]++;
    if (defined $value){
      # values are positive if good for player to move.
      $X_wins[$depth]++ if ( $value > 0 and $whoseturn eq 'X' );
      $O_wins[$depth]++ if ( $value < 0 and $whoseturn eq 'X' );
      $O_wins[$depth]++ if ( $value > 0 and $whoseturn eq 'O' );
      $X_wins[$depth]++ if ( $value < 0 and $whoseturn eq 'O' );
      
    }
  }
  print "    depth  positions  X_can_win  O_can_win  draws \n";
  # Below I use printf(), the formatted print statement, 
  # to make the various columns line up.  
  # "%5d", for example, means "a number padded to 5 chars wide".
  for my $depth (0..9){
    printf( "    %5d  %9d  %9d  %9d  %5d \n", 
            $depth, $counts[$depth], $X_wins[$depth], $O_wins[$depth],
            ($counts[$depth] - ($X_wins[$depth]+$O_wins[$depth])));
  }
  print "    Here's a sample of the stored positions and best moves. \n";
  print "    The value is positive if the player to move can force a win.\n";
  print "    position       bestmove  value  \n";
  my $counter = 0;
  foreach my $key (sort keys %memo_data){
    my $boardstring = $key;
    $boardstring =~ s/ /\./g;        # change spaces to periods
    substr($boardstring,6,0) = ' ';  # insert spaces at positions 6,3
    substr($boardstring,3,0) = ' ';
    $counter++;
    last if $counter > 20;
    my ($value, $move) = @{$memo_data{$key}};
    $value = 'undef' if not defined $value; 
    printf("    %8s    %8d  %5d \n", 
           $boardstring, $move, $value);
  }
  print "\n";
}


#
# ==== testing / debugging =========================================

# Here's where I define various tests that ensure the subroutines
# are doing what I expect.  Each test is a boolean expression that
# should evaluate true if things are working as I expect.
# They also give a sense of how the subroutines can be called.
#
# When the tests run, the output on the screen looks something like
#   1 ok            ok(1) works.
#   2 ok            test number is incrementing properly.
#   3 not ok        this test failed.
#
# I wrote these tests as I wrote the subroutines, 
# to make sure that each was working as it should.
# And I've tried to keep as much debugging material here, in one place,
# though I did drop a few "print" statements into the code at times.

# Usage:   ok( something() == $expected,   "descriptive comment");
# Effect:  prints out a line like "1 ok     this test worked".
# Modifies global variables $test_number, $failed_tests.
sub ok {
  my ($success, $comment) = @_;
  $test_number++;
  $failed_tests++ unless $success;
  my $output = " " . $test_number;
  $output   .= " not" unless $success;
  $output   .= " ok";
  print $output;
  # Print the comment (if any) starting at column twenty
  # by outputting 21-length($output) spaces, 
  # where $output is something like "12 not ok".
  print " "x(21-length($output)) . $comment if $comment;
  print "\n";
}

# So let's see what those subroutines can do...
# Usage: run_tesst();
sub run_tests {
  # -- testing ok() itself.
  my ($true, $false) = (1,0);
  ok($true,  "Testing ok itself with ok(1,comment).");
  ok($true,  "Is the test number incrementing properly?");
  # ok($false, "This test should fail.  (Is the comment still aligned?)");

  # -- check to see if globals look ok --
  init_game();
  ok( $board[2]   eq ' ',     "board looks blank");
  ok( $letters[0] eq 'a',     "0'th letter is a");
  ok( $numbers{a} == 0,      "a'th number is 0");

  # -- change $user_tests=1 to run the interactive tests.
  # (Once I see these look all right, I turn them off to avoid doing input.)
  my $user_tests = 0;
  @board = ('X', 'X', 'X', ' ', '0', 'X', 'O', ' ' , 'O');  # 3, 7 are legal.
  if ($user_tests){
    print " in run_tests:\n" . board_as_text();
    my $move = get_user_move();
    ok( $move==3 || $move ==7, "legal move entered.");
    my @p = choose_players();
    print " in run_tests: players chosen are (@p)\n";
  }

  # -- check that get_random_moves delivers legal moves.
  my $legal = 1;
  for (1..10){
    my $move = get_random_move();
    $legal = 0 unless $move == 3 or $move == 7;   # see @board=(...) above
  }
  ok($legal,  "get_random_move");

  # -- is find winner working ?
  @board = ('X', ' ', 'X', 
            'O', '0', 'X', 
            'O', ' ', 'X');
  ok( 'X' eq find_winner(),  "find_winner 'X'");
  @board = (' ', 'O', ' ',  
            ' ', 'O', ' ', 
            ' ', 'O', ' ');
  ok( 'O' eq find_winner(),  "find_winner 'O'");
  init_game();
  ok( ! find_winner(),       "no winner in initial position");
  @board = ('X', 'O', ' ',  
            'X', ' ', 'O', 
            'X', ' ', ' ');
  ok( 'X' eq find_winner(),  "find_winner 'X' left vertical");

  # -- check do_move and undo_move
  init_game();
  do_move(0); do_move(1);    # X 0 .
  do_move(3); do_move(4);    # X 0 0
  do_move(6); do_move(5);    # X . .     #  6 moves have been made.
  ok( whose_move() eq 'X',  "do_move: after moves 0,1,3,4,6,5, it's X's move");
  ok( 'X' eq find_winner(), " and X has won");
  undo_move(); undo_move();
  ok( ! find_winner(),      "undo_move: after undoing two moves, no winner");
  ok( 4 == scalar(@moves),  " and number of moves is 4.");

  # -- searching for best move
  init_game();
  do_move(0); do_move(1);    # X 0 .
  do_move(3); do_move(5);    # X 0 0
  do_move(6); do_move(4);    # X . .     #  6 moves have been made.
  # print board_as_text();
  # print "   in tests: evaluate is '" , evaluate(), "'\n";
  my ($value) = evaluate();
  ok( 4 == $value,               "evaluation of X win; X's point of view");
  undo_move(); undo_move();
  $value = evaluate();
  ok( ! defined $value,           "ongoing game evaluates to undef");

  # -- Was a problem here : doesn't find winning move; finds 4 not 6 --
  # -- resolution: DOES find a winning move, just not the
  #    one I expected, because it doesn't look for the shortest win.
  # -- solution: modify evaluation routine to prefer wins in less moves.
  #    let value = +-1 * ( 10 - scalar(@moves) ) 
  init_game();  do_move(0); do_move(1); do_move(3); do_move(5);
  # print "  in tests:\n";
  # print board_as_text();
  # print "  in tests: moves are (@moves)\n";
  my $move = get_minmax_move();
  # print "  in tests: minmax best move is '$move'\n";
  ok( 6 == $move,   "minmax finds best X move");

  # -- same test for alphabeta  --
  #    was the same unexpected result here, namely move 4.
  #    now does as expected with evaluate() looking for quicker wins.
  # print "  in tests:\n";
  # print board_as_text();
  # print "  in tests: moves are (@moves)\n";
  $move = get_alphabeta_move();
  # print "  in tests: alphabeta move is '$move'\n";
  ok( 6 == $move,   "alphabeta finds best X move");

  # -- check more win/lose situations --
  do_move(2);
  # print board_as_text();
  ok( 6 == get_alphabeta_move(), "alphabeta finds blocking O move");

  # -- memoization --
  %memo_data = ();  # forget any previous memory of searches.
  @board = ('X','X',' ', 'O','O',' ', ' ',' ',' ');
  my $boardkey = board_as_key();
  # print "  in test: boardkey = '$boardkey'\n";
  ok( 'XX OO    ' eq $boardkey,            "board_as_key");
  ok( ! memo_exists($boardkey),            "not memo_exists");
  memo_store(undef, 2, $boardkey);
  ok( memo_exists($boardkey),              "memo_exists after memo_store");
  ($value,$move) = memo_get($boardkey);
  ok( (! defined $value and $move == 2),   "memo_get");
   
  # -- memo searching --
  %memo_data = ();  # forget any previous memory of searches.
  init_game();
  # Note that searches require @board and @moves to be consistent;
  # just setting @board won't work.
  do_move(0); do_move(3);
  do_move(1); do_move(4);    # position is "X,X, ,O,O, , , , "
  ($value,$move) = memoalphabeta(-$infinity,$infinity);
  ok( $move == 2,    "memoalphabeta");
  ok( $move == 2,    "still memoalphabeta");
  # print " in test: after alphabeta size of memo_data = ";
  # print   scalar(keys %memo_data), ", move = $move\n";

  #       =====>  add more tests here   <=====

  # -- finished testing

  if ($failed_tests){
    my $s = $failed_tests == 1 ? '' : 's';          # singular or plural.
    print " ** Failed $failed_tests test$s. **\n";
  }
  else {
    print " Passed all tests.\n";
  }
}

# == POD ===================================================

__END__

=head1 NAME

B<TicTacToe.pl> - a text version of a simple two player game in Perl.

=head1 SYNOPSIS

To run the program, at the command prompt type

 $ ./TicTacToe.pl

A sample of dialog that can follow is given below.

=head1 DESCRIPTION

The program plays a game of Tic Tac Toe through
a text interface.  The moves for the X and O players
can be input from the keyboard or generated by 
several algorithms, including random moves, 
a Min/Max recursive search, or an Alpha/Beta pruned
search.

While having a computer play a game of Tic Tac Toe doesn't
stretch the limits of artificial intelligence, this code
does at least give an example the pieces that go
into a program that is more than a trivial exercise.
Because of that, many programming classes use an example like this on
as a project assignment.

I've tried to keep the Perl code fairly straightforward
(i.e. no objects, all the code in one file, 
no external packages, and not I<too> many arcance perl idioms),
so that this example won't be too extreme for my intro programming 
students.  Whether I've succeeded in this attempt is subject to debate;
the whole thing has grown rather large at about 1500 lines.

Since I've been ragging on my students about documentation 
and tests, I've put plenty of both in here. 
While this level of verbiage and quality control may be a bit 
over the top, but getting into the right habits really is, IMHO, 
a really good idea, and saves time in the long run.
Even a program as simple as this one is built up with
layers of subroutines, in the same way that a building is
built with bricks; if the pieces aren't solid then the
whole thing will likely collapse as it grows larger.

The L<SAMPLE OUTPUT> section below shows what it all does.

So there you are.

=head1 DOCUMENTATION

Implementation notes, comments on the data structures used,
a description of how to go about searching for the best move
in two player games, and the
API's (Application Programmer's Interface) for the subroutines
are all embedded in the source code.  So use the source, Luke.

To create this .html overview
from the POD (Plain Old Documentation) at the 
end of the TicTacToe.pl source file, 
type the following at the command prompt.

 pod2html TicTacToe.pl > TicTacToe.html

=head1 SAMPLE OUTPUT

 [command_prompt]$ ./TicTacToe.pl

 -- Tic Tac Toe --    version 0.3 10/21/03 

 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store best moves; summarize results
   quit     - quit
 What would you like to do ? t
 1 ok                Testing ok itself with ok(1,comment).
 2 ok                Is the test number incrementing properly?
 3 ok                board looks blank
 4 ok                0'th letter is a
 5 ok                a'th number is 0
 6 ok                get_random_move
 7 ok                find_winner 'X'
 8 ok                find_winner 'O'
 9 ok                no winner in initial position
 10 ok               find_winner 'X' left vertical
 11 ok               do_move: after moves 0,1,3,4,6,5, it's X's move
 12 ok                and X has won
 13 ok               undo_move: after undoing two moves, no winner
 14 ok                and number of moves is 4.
 15 ok               evaluation of X win; X's point of view
 16 ok               ongoing game evaluates to undef
     Starting minmax search ... 
     ... done.
     182 total positions examined in about a second.
     Breakdown by depth is (1 5 16 48 60 52).
 17 ok               minmax finds best X move
     Starting alphabeta search ... 
     ... done.
     82 total positions examined in less than a second.
     Breakdown by depth is (1 5 10 24 21 21).
 18 ok               alphabeta finds best X move
     Starting alphabeta search ... 
     ... done.
     33 total positions examined in less than a second.
     Breakdown by depth is (1 4 8 10 10).
 19 ok               alphabeta finds blocking O move
 20 ok               board_as_key
 21 ok               not memo_exists
 22 ok               memo_exists after memo_store
 23 ok               memo_get
 24 ok               memoalphabeta
 25 ok               still memoalphabeta
 Passed all tests.

 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store (value,best_move) for every position; summarize
   quit     - quit
 What would you like to do ? a
 Analyzing all possible board positions (this takes a few minutes ) ... 
 ... done; considered 986410 positions in 235 seconds.
 Computer player should now respond with a good moves instantly.
    Number of distinct positions found = 6046.
    depth  positions  X_can_win  O_can_win  draws 
        0          1          0          0      1 
        1          9          0          0      9 
        2         72         48          0     24 
        3        252         64         50    138 
        4        756        584         36    136 
        5       1260        456        540    264 
        6       1680       1210        270    200 
        7       1260        542        518    200 
        8        630        346        204     80 
        9        126         80         30     16 
    Here's a sample of the stored positions and best moves. 
    The value is positive if the player to move can force a win.
    position       bestmove  value  
    ... ... ...           0      0 
    ... ... ..X           4      0 
    ... ... .OX           2      3 
    ... ... .X.           1      0 
    ... ... .XO           0      0 
    ... ... O.X           0      3 
    ... ... OX.           0      0 
    ... ... OXX           0      2 
    ... ... X..           4      0 
    ... ... X.O           0      3 
    ... ... XO.           0      3 
    ... ... XOX           4      0 
    ... ... XXO           2      2 
    ... ..O ..X           4      3 
    ... ..O .X.           4      3 
    ... ..O .XX           6     -3 
    ... ..O OXX           4      3 
    ... ..O X..           0      3 
    ... ..O X.X           7     -3 
    ... ..O XOX           0      3 


 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store (value,best_move) for every position; summarize
   quit     - quit
 What would you like to do ? p
 
 The first player is X; the second is O.
 Each can be one any of (user computer random minmax alphabeta).
 Type 'help' or 'h' for an explanation of the choices.
 Player X is ? c
 OK, X is 'computer'.
 Player O is ? r
 OK, O is 'random'.

      |   |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 


 Computer playing 'X' chooses 'h'.

      |   |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      | X |          g | h | i 


 Random computer playing 'O' chooses 'b'.

      | O |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      | X |          g | h | i 


 Computer playing 'X' chooses 'a'.

    X | O |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      | X |          g | h | i 


 Random computer playing 'O' chooses 'c'.

    X | O | O        a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      | X |          g | h | i 


 Computer playing 'X' chooses 'g'.

    X | O | O        a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
    X | X |          g | h | i 


 Random computer playing 'O' chooses 'f'.

    X | O | O        a | b | c 
   ---+---+---      ---+---+---
      |   | O        d | e | f 
   ---+---+---      ---+---+---
    X | X |          g | h | i 


 Computer playing 'X' chooses 'd'.

    X | O | O        a | b | c 
   ---+---+---      ---+---+---
    X |   | O        d | e | f 
   ---+---+---      ---+---+---
    X | X |          g | h | i 


   ==> X wins! <== 

 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store (value,best_move) for every position; summarize
   quit     - quit
 What would you like to do ? p
 
 The first player is X; the second is O.
 Each can be one any of (user computer random minmax alphabeta).
 Type 'help' or 'h' for an explanation of the choices.
 Player X is ? h

  The methods of choosing the next move are
   user       - you input moves from the keyboard
   computer   - random first X move, then memoalphabeta search  (i.e. smart)
   random     - randomly chosen legal move                      (i.e. stupid)
   minmax     - min/max recursive search with positions and time summary
   alphabeta  - alpha/beta pruned search with positions and time summary 

 Player X is ? a
 OK, X is 'alphabeta'.
 Player O is ? m
 OK, O is 'minmax'.

      |   |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 

     Starting alphabeta search ... 
     ... done.
     20866 total positions examined in 4 seconds.
     Breakdown by depth is (1 9 30 132 454 1597 2948 6172 5911 3612).

 AlphaBeta computer playing 'X' chooses 'a'.

    X |   |          a | b | c 
   ---+---+---      ---+---+---
      |   |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 

     Starting minmax search ... 
     ... done.
     59705 total positions examined in 10 seconds.
     Breakdown by depth is (1 8 56 336 1680 6000 16344 21312 13968).

 Minmax computer playing 'O' chooses 'e'.

    X |   |          a | b | c 
   ---+---+---      ---+---+---
      | O |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 

     Starting alphabeta search ... 
     ... done.
     871 total positions examined in less than a second.
     Breakdown by depth is (1 7 19 67 112 270 263 132).

 AlphaBeta computer playing 'X' chooses 'b'.

    X | X |          a | b | c 
   ---+---+---      ---+---+---
      | O |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 

     Starting minmax search ... 
     ... done.
     935 total positions examined in less than a second.
     Breakdown by depth is (1 6 30 100 258 360 180).

 Minmax computer playing 'O' chooses 'c'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
      | O |          d | e | f 
   ---+---+---      ---+---+---
      |   |          g | h | i 

     Starting alphabeta search ... 
     ... done.
     64 total positions examined in less than a second.
     Breakdown by depth is (1 5 12 20 20 6).

 AlphaBeta computer playing 'X' chooses 'g'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
      | O |          d | e | f 
   ---+---+---      ---+---+---
    X |   |          g | h | i 

     Starting minmax search ... 
     ... done.
     47 total positions examined in less than a second.
     Breakdown by depth is (1 4 12 18 12).

 Minmax computer playing 'O' chooses 'd'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
    O | O |          d | e | f 
   ---+---+---      ---+---+---
    X |   |          g | h | i 

     Starting alphabeta search ... 
     ... done.
     10 total positions examined in less than a second.
     Breakdown by depth is (1 3 4 2).

 AlphaBeta computer playing 'X' chooses 'f'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
    O | O | X        d | e | f 
   ---+---+---      ---+---+---
    X |   |          g | h | i 

     Starting minmax search ... 
     ... done.
     5 total positions examined in less than a second.
     Breakdown by depth is (1 2 2).

 Minmax computer playing 'O' chooses 'h'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
    O | O | X        d | e | f 
   ---+---+---      ---+---+---
    X | O |          g | h | i 

     Starting alphabeta search ... 
     ... done.
     2 total positions examined in less than a second.
     Breakdown by depth is (1 1).

 AlphaBeta computer playing 'X' chooses 'i'.

    X | X | O        a | b | c 
   ---+---+---      ---+---+---
    O | O | X        d | e | f 
   ---+---+---      ---+---+---
    X | O | X        g | h | i 


   ==> Draw! <==

 Please type one of the following choices or its first letter.
   play     - play a game
   tests    - run the diagnostics
   analyze  - find and store best moves; summarize results
   quit     - quit
 What would you like to do ? q
 Are you sure you want to quit the program? [y] 

 Bye.

 [command_prompt]$ 

=head1 VERSION HISTORY

 v0.1  Oct 16 2003
  * First attempt
  * Board as 3x3 grid $board[$row][$column]
  * Search for winner with loops 
  * Board printed with loops.
  * Move chosen as letter 'a'..'i'
  * User and random computer players
  * But ... feels overblown and messy for a simple example.

 v0.2  Oct 19
  * Rewrote board as @board[0..8];
  * Rewrote search for winner using 8 explicit triples.
  * Rewrote printing of board using explicit embedded scalars in string.
  * Added statistics
  * Added minmax and alphabeta search
  * Modified evaluation() to look for shortest win, not just any win.

 v0.3  Oct 21
  * Added "memoization" ability to remember previous search result.

 v0.31 Oct 22
  * Edited some of the comments and documentation.
  * Translated the sample computer search code in the comments from C to Perl.

=head1 SEE ALSO

=over

=item alpha/beta search

The alpha/beta algorithm is the standard way to search
for the best move in a two player game.  There are many good
descriptions available online; just ask Google.  
I used Bruce Moreland's description and code template 
(L<http://www.seanet.com/~brucemo/topics.html>) to remind myself
of how it works.

=item Plain Old Documentation

You can read about POD
(the typical Perl documentation markup language
used to make what you're reading)
at L<http://perldoc.com/perl5.8.0/pod/perlpod.html>.

=item Perl

For more Perl documentation, try
the online manual at L<http://perldoc.com/perl5.8.0/pod/perl.html>.

=back

=head1 AUTHOR, COPYRIGHT, LICENSE

 Jim Mahoney <mahoney@marlboro.edu> 
 Marlboro College
 Copyright 2003

This is free software; you can redistribute it and/or modify
it under the same terms as Perl itself. 

=cut
