#!/usr/bin/env python
"""
 machinarium_puzzle.py

 There are three snakes, one green, one orange, one purple,
 positioned as shown below. Each moves as a curvey line
 from one of its ends (a lower case letter) into one of
 the five empty spots (periods).

 The object is to get the greens into the bottom right.

 >>> p = Puzzle()
 >>> print p,
        O O o p
    g . O     P
    G   O     P
    g . o P P P
      p   P    
    . P P P . .
 >>> p.do_move(((3,0), (3,1)))
 >>> print p,
        O O o p
    . . O     P
    g   O     P
    G g o P P P
      p   P    
    . P P P . .
 >>> p.undo_move()
 >>> print p,
        O O o p
    g . O     P
    G   O     P
    g . o P P P
      p   P    
    . P P P . .

  Jim Mahoney | Dec 2010 | GPL
"""
import doctest

class Puzzle:
  """ A sliding snakey puzzle from the Machinarium game.  """

  N = 6   # board width and height
  solved_char = 'g'     # puzzle is solved when this char
  solved_coord = (5,5)  # is in this position
  empty = '.'
  nonboard = ' '

  def __init__(self):
    self.history = []     # a list of snakes ; see move2snake below
    self.board = self.initial_board()
    self.coords = self.board_coords()
    self.snakes = self.initial_snakes()
    self.seen = set()     # boards seen for search

  def board_coords(self):
    """ Return a set of (row,col) board coordinates. """
    coords = set()
    for i in range(Puzzle.N):
      for j in range(Puzzle.N):
        coord = (i, j)
        if self(coord) != Puzzle.nonboard:
          coords.add(coord)
    return coords

  def initial_board(self):
    return [[' ', ' ', 'O', 'O', 'o', 'p'],
            ['g', '.', 'O', ' ', ' ', 'P'],
            ['G', ' ', 'O', ' ', ' ', 'P'],
            ['g', '.', 'o', 'P', 'P', 'P'],
            [' ', 'p', ' ', 'P', ' ', ' '],
            ['.', 'P', 'P', 'P', '.', '.']]

  def initial_snakes(self):
    """ Return dictionary of snake coordinate lists. """
    snakes = {'g':None, 'o':None, 'p':None}
    for coord in self.coords:
      char = self(coord)
      if char in ('g', 'o', 'p') and not snakes[char]:
        snakes[char] = self.follow_snake(coord)
    return snakes

  def __str__(self):
    result = ''
    for row in self.board:
      result += '   ' + ' '.join(row) + '\n'
    return result

  def __call__(self, *args):
    """ Return character at given board coordinate.
        >>> p = Puzzle()
        >>> p(0, 2)
        'O'
        >>> p((0,2))
        'O'
    """
    if len(args) == 2:
      return self.board[args[0]][args[1]]
    else:
      return self.board[args[0][0]][args[0][1]]

  def put(self, char, coord):
    """ Put a character into a board coordinate. """
    self.board[coord[0]][coord[1]] = char

  def neighbors(self, coord):
    """ Return the two to four neighboring cell coordinates.
        >>> p = Puzzle()
        >>> n = p.neighbors((0, 2))
        >>> n == [(0, 3), (1, 2)] or n == [(1, 2), (0, 3)]
        True
    """
    result = []
    for (delta_i, delta_j) in  [(1,0), (0,1), (0,-1), (-1,0)]:
      neighbor = (coord[0] + delta_i, coord[1] + delta_j)
      if neighbor in self.coords:
        result.append(neighbor)
    return result

  def moves(self):
    """ Return list of possible moves (coords of lowercase to period)
        in the form [((i1,j1), (i2,j2)), ...] 
        >>> p = Puzzle()
        >>> len(p.moves())
        4
        >>> ((3,0), (3,1)) in p.moves()
        True
    """
    result = []
    for coord in self.coords:
      if self(coord).islower():
        for neighbor in self.neighbors(coord):
          if self(neighbor) == Puzzle.empty:
            result.append((coord, neighbor))
    return result

  def move2blanksnake(self, move):
    """ Given a move (from, to) with e.g. self(from)=='g' and self(to)=='.',
        return the coordinates of ['.', 'g', 'G', 'g'].
        >>> p = Puzzle()
        >>> p.move2blanksnake(((3,0), (3,1)))
        [(3, 1), (3, 0), (2, 0), (1, 0)]
    """
    char = self(move[0])
    snake = self.snakes[char]
    if snake[0] != move[0]:
      snake.reverse()
    return [move[1]] + snake

  def follow_snake(self, start_coord):
    """ Return list of coords where snake is e.g. ['g', 'G', 'g'].
        Fails if path of the snake isn't obvious (i.e. single valued). """
    end_char = self(start_coord)
    mid_char = end_char.upper()
    snake = [start_coord]
    max_iter = 30
    while True:
      max_iter -= 1
      if max_iter < 0:
        return None
      for neighbor in self.neighbors(snake[-1]):
        # print "neighbor " , neighbor
        if len(snake) < 2 or neighbor != snake[-2]:
          if self(neighbor) == mid_char:
            snake.append(neighbor)
            break
          elif self(neighbor) == end_char:
            snake.append(neighbor)
            return snake

  def slide(self, snake):
    """ Modify self.board and self.snakes by sliding a snake one place. """
    # Since the snake's coords includes the empty at the end,
    # all we need to do is flip it and put it back onto the board.
    # Sliding the same snake again undoes it.
    chars = [self(coord) for coord in snake]   # e.g. ['.', 'g', 'G', 'g']
    chars.reverse()                            # e.g. ['g', 'G', 'g', '.']
    for (coord, char) in zip(snake, chars):
      self.put(char, coord)
    if chars[0].islower():
      self.snakes[chars[0]] = snake[:-1]
    else:
      self.snakes[chars[-1]] = snake[1:]

  def do_move(self, move):
    """ Apply the given move to the current board and remember it. """
    snake = self.move2blanksnake(move)
    self.history.append(snake)
    self.slide(snake)

  def undo_move(self):
    """ Undo the last move. """
    snake = self.history.pop()
    self.slide(snake)
    
  def is_solved(self):
    """ Return True if the puzzle is in its solved state. """
    return self(Puzzle.solved_coord) == Puzzle.solved_char

  def print_history(self):
    """ Print board position history. """
    p = Puzzle()
    print p
    for snake in self.history:
      p.slide(snake)
      print p

  def search(self, depth):
    """ Recursive search for solution. """
    # depth_first, backtrack, no repeated positions, depth limited
    #
    # A breadth first search might be better, but would require that
    # the search tree have contain puzzles with complete state,
    # not just do/undo moves on one puzzle.
    # The approach here uses less memory but is necessarily depth-first.
    # Using a 'fringe' of puzzled to be searched
    # as a queue gives breadth-first search;
    # as a stack gives depth-first.
    #
    ## print " "*len(self.history) + str(len(self.seen))  ## debug
    if depth < 0:
      return
    self_string = str(self)                #
    if self_string in self.seen:           # Don't search positions
      return                               # which have been seen before.
    else:                                  #
      ## print self_string  ## debug       #
      self.seen.add(self_string)           #
    if self.is_solved():
      raise Exception('done')
    else:
      for move in self.moves():
        self.do_move(move)
        self.search(depth - 1)
        self.undo_move()

  def solve(self):
    """ Iterative deepening search """
    try:
      depth = 1
      while True:
        self.seen = set()
        self.search(depth)
        depth += 1
    except:
      print "Found solution with %i moves :" % len(self.history)
      self.print_history()

if __name__ == '__main__':
  doctest.testmod()
  Puzzle().solve()
  



