var WIDTH = 900
var HEIGHT = 500

var ROOT_X = 30
var NODE_SIZE = 35
var NODE_DX = 50
var NODE_DY = 75

var TRANSITION = 700

var RED = 'red'
var GREEN = 'green'
var YELLOW = 'yellow'
var PURPLE = 'purple'

function assert(condition) {
  if (!condition) throw new Error()
}

// stolen from http://bl.ocks.org/NPashaP/7683252, and modified
function make_naive_tree() {
  var nodeSize = 40
  var nodeDx = NODE_DY
  var nodeDy = NODE_DX
  var rootY = ROOT_X
  var tree = {}
  tree.HISTORY = 4

  var reset = function() {
    tree.roots =
      [ { id : 0
        , label : 0
        , active : true
        , children : [] } ]
    tree.next_id = 1
    reposition()
  }

  var garbageCollect = function() {
    var new_roots = []
    function gc(underGarbage, u) {
      if (underGarbage && u.label < tree.HISTORY) {
        new_roots.push(u)
      }
      u.children.forEach(function(v) {gc(u.label>=tree.HISTORY, v)})
      u.children = u.children.filter(function(v) {return v.label < tree.HISTORY})
    }
    tree.roots.forEach(function(u) { gc(true, u) })
    tree.roots = new_roots
    reposition()
  }

  tree.getVertices = function() {
    var vs = []
    function getVertices(u, startPosition) {
      vs.push(
        { id : u.id
        , label : u.label
        , position : u.position
        , startPosition : startPosition }
      )
      u.children.forEach(function(v) { getVertices(v, u.position) })
    }
    tree.roots.forEach(function(v) { getVertices(v, v.position) })
    return vs.sort(function(a,b){ return a.id - b.id })
  }

  tree.getEdges = function() {
    var es = []
    function getEdges(_) {
      _.children.forEach(function(d) {
        es.push(
          { v1:_.id
          , l1:_.label
          , p1:_.position
          , v2:d.id
          , l2:d.label
          , p2:d.position }
        )
      })
      _.children.forEach(getEdges);
    }
    tree.roots.forEach(getEdges)
    return es.sort(function(a,b){ return a.v2 - b.v2;});
  }

  var modifyVertex = function(id, f) {
    function mv(t) {
      if (t.id == id) f(t)
      t.children.forEach(mv)
    }
    tree.roots.forEach(mv)
  }

  tree.addLeaf = function(v){
    modifyVertex(v, function (t) {
      if (t.active) {
        t.children.push(
          { id : tree.next_id++
          , label : 0
          , active : true
          , children : [] }
        )
      }
    })
    reposition()
    relabel()
    redraw()
  }

  tree.deactivate = function(id) {
    modifyVertex(id, function(v) { v.active = false })
    relabel()
    redraw()
  }

  var clickHandler = function(vertex) {
    if (d3.event.ctrlKey) {
      tree.deactivate(vertex)
    } else {
      tree.addLeaf(vertex)
    }
  }

  var relabel = function() {
    function relbl(u) {
      u.children.forEach(relbl)
      if (u.active) {
        u.label = 0
      } else {
        u.label = 1000
        u.children.forEach(function(v) { u.label = Math.min(u.label, v.label) })
        u.label += 1
      }
    }
    tree.roots.forEach(relbl)
  }

  var vertexcolor = function(vertex) {
    if (vertex.label == 0) {
      return GREEN
    } else if (vertex.label < tree.HISTORY) {
      return YELLOW
    } else {
      return RED
    }
  }

  var vertextext = function(vertex) {
    if (vertex.label >= 1000) return '&infin;'
    return vertex.label
  }

  var redraw = function() {
    var edges = d3.select("#g_lines").selectAll('line')
      .data(tree.getEdges(), function(d) {return 'from '+d.v1+' to '+d.v2})
    edges.transition().duration(TRANSITION)
      .attr('x1',function(d){ return d.p1.y }).attr('y1',function(d){ return d.p1.x })
      .attr('x2',function(d){ return d.p2.y }).attr('y2',function(d){ return d.p2.x })
    edges.enter().append('line')
      .attr('x1',function(d){ return d.p1.y }).attr('y1',function(d){ return d.p1.x })
      .attr('x2',function(d){ return d.p1.y }).attr('y2',function(d){ return d.p1.x })
      .transition().duration(TRANSITION)
      .attr('x2',function(d){ return d.p2.y }).attr('y2',function(d){ return d.p2.x })
    edges.exit().remove()

    var nodes = d3.select("#g_nodes").selectAll('rect')
      .data(tree.getVertices(), function(d) {return d.id})
    nodes.transition().duration(TRANSITION)
      .attr('x',function(d){ return d.position.y - nodeSize / 2 })
      .attr('y',function(d){ return d.position.x - nodeSize / 2 })
      .style('fill', vertexcolor)
    nodes.enter().append('rect')
      .attr('x', function(d) { return d.startPosition.y - nodeSize / 2 })
      .attr('y',function(d){ return d.startPosition.x - nodeSize / 2 })
      .attr('height', nodeSize).attr('width', nodeSize)
      .style('fill', vertexcolor)
      .on('click',function(d){ return clickHandler(d.id) })
      .transition().duration(TRANSITION)
      .attr('x',function(d){ return d.position.y - nodeSize / 2 })
      .attr('y',function(d){ return d.position.x - nodeSize / 2 })
    nodes.exit().remove()

    var labels = d3.select("#g_labels").selectAll('text')
      .data(tree.getVertices(), function(d) {return d.id})
    labels.html(vertextext).transition().duration(TRANSITION)
      .attr('x',function(d){ return d.position.y })
      .attr('y',function(d){ return d.position.x+nodeSize/2-7 })
    labels.enter()
      .append('text')
      .attr('x',function(d){ return d.startPosition.y })
      .attr('y',function(d){ return d.startPosition.x+nodeSize/2-7 })
      .html(vertextext)
      .on('click',function(d){return clickHandler(d.id) })
      .transition().duration(TRANSITION)
      .attr('x',function(d){ return d.position.y })
      .attr('y',function(d){ return d.position.x+nodeSize/2-7 })
    labels.exit().remove()
  }

  var reposition = function() {
    function computeLeafCount(u) {
      u.children.forEach(computeLeafCount)
      u.leafCount = 0
      u.children.forEach(function(v) { u.leafCount += v.leafCount })
      u.leafCount = Math.max(1, u.leafCount)
    }
    tree.roots.forEach(computeLeafCount)
    function setPosition(offsetX, offsetY, u) {
      u.position =
        { x : offsetX + u.leafCount * nodeDx / 2
        , y : offsetY }
      u.children.forEach(function(v) {
        setPosition(offsetX, offsetY + nodeDy, v)
        offsetX += v.leafCount * nodeDx
      })
    }
    totalLeafCount = 0
    tree.roots.forEach(function(u) {totalLeafCount += u.leafCount})
    offsetX = HEIGHT / 2 - totalLeafCount * nodeDx / 2
    tree.roots.forEach(function(u) {
      setPosition(offsetX, rootY, u)
      offsetX += u.leafCount * nodeDx
    })
  }

  var initialize = function() {
    reset()
    d3.select("#treebuffer-naive")
      .append("svg").attr("width", WIDTH).attr("height", HEIGHT)
      .attr('id', 'treesvg')
    d3.select("#treebuffer-naive").append("div").attr('id', 'navdiv')
    d3.select("#navdiv")
      .append("button").attr('type', 'button').text('Reset')
      .on('click', function(_) { reset(); redraw() })
    d3.select("#navdiv")
      .append("button").attr('type', 'button').text('Collect')
      .on('click', function(_) { garbageCollect(); redraw() })
    d3.select("#treesvg").append('g').attr('id', 'g_lines')
    d3.select("#treesvg").append('g').attr('id', 'g_nodes')
    d3.select("#treesvg").append('g').attr('id', 'g_labels')
    redraw()
  }

  initialize()
  return tree
}
var naive_tree = make_naive_tree()

// This one is inspired by the function from above, but mostly rewritten.
function make_realtime_tree() {
  var tree = { HISTORY : 4 }

  var check_tree = function() {
    children_count = 0
    roots_count = 0
    tree.nodes.forEach(function(u) {
      if (u.parent == null) {
        roots_count += 1
      }
      u.children.forEach(function(v) {
        children_count += 1
        assert (u == v.parent)
      })
    })
    assert (tree.nodes.length == roots_count + children_count)

    tree.nodes.forEach(function(v) {
      assert (v.representant.depth % tree.HISTORY == 0)
    })

    tree.nodes.forEach(function(v) {
      v.active_count_check = 0
    })
    tree.nodes.forEach(function(v) {
      if (v.active) {
        v.representant.active_count_check += 1
      }
    })
    tree.nodes.forEach(function(v) {
      assert (v.active_count == v.active_count_check)
    })

    tree.to_delete.forEach(function(v) {
      assert (v.to_delete)
      assert (!v.active)
    })
    to_delete_count = 0
    tree.nodes.forEach(function(v) {
      if (v.to_delete) {
        to_delete_count += 1
      }
    })
    assert (to_delete_count == tree.to_delete.length)
  }

  var reset = function() {
    root =
      { parent : null
      , children : []
      , depth : 0
      , active_count : 1
      , active : true
      , to_delete : false
      , id : 0 }
    root.representant = root
    tree.nodes = [root]
    tree.to_delete = []
    tree.next_id = 1
    check_tree()
  }

  var clickHandler = function(v) {
    if (d3.event.ctrlKey) {
      deactivate(v)
    } else {
      addLeaf(v)
    }
    redraw()
  }

  var deactivate = function(v) {
    if (!v.active) return
    v.active = false
    v.representant.active_count -= 1
    if (v.children.length == 0) {
      enque(v)
    }
    if (v.representant.active_count == 0) {
      cut_parent(v.representant)
    }
    process_queue()
    check_tree()
  }

  var addLeaf = function(v) {
    if (!v.active) return;
    w =
      { parent : v
      , children : []
      , depth : v.depth + 1
      , active : true
      , active_count : 0
      , to_delete : false
      , id : tree.next_id++ }
    tree.nodes.push(w)
    v.children.push(w)
    if (w.depth % tree.HISTORY == 0) {
      w.representant = w
    } else {
      w.representant = v.representant
    }
    w.representant.active_count += 1
    process_queue()
    check_tree()
  }

  var process_queue = function() {
    if (tree.to_delete.length > 0) {
      // TODO do this stuff in constant time
      tree.to_delete.reverse()
      v = tree.to_delete.pop()
      tree.to_delete.reverse()
      cut_parent(v)
      tree.nodes = tree.nodes.filter(function(u) { return u.id != v.id })
    }
  }

  var enque = function(v) {
    assert (!v.to_delete)
    v.to_delete = true
    tree.to_delete.push(v)
  }

  var cut_parent = function(v) {
    u = v.parent
    if (u != null) {
      u.children = u.children.filter(function(w) { return w.id != v.id})
      if (u.children.length == 0 && !u.active) {
        enque(u)
      }
    }
    v.parent = null
  }

  var getRepArcs = function() {
    var ps = []
    tree.nodes.forEach(function(v) {
      if (v.active) {
        u = v.representant
        ps.push(
          { x1 : v.position.x
          , y1 : v.position.y
          , x2 : u.position.x
          , y2 : u.position.y
          , id : 'from ' + v.id + ' to ' + u.id }
        )
      }
    })
    return ps
  }

  var getParentLinks = function() {
    var ps = []
    tree.nodes.forEach(function(v) {
      u = v.parent
      if (u != null) {
        ps.push(
          { x1 : u.position.x
          , y1 : u.position.y
          , x2 : v.position.x
          , y2 : v.position.y
          , id : 'from ' + u.id + ' to ' + v.id }
        )
      }
    })
    return ps
  }

  var redraw = function() {
    // compute positions
    roots = tree.nodes.filter(function(v) { return v.parent == null })
    function computeLeafCount(u) {
      u.children.forEach(computeLeafCount)
      u.leafCount = 0
      u.children.forEach(function(v) { u.leafCount += v.leafCount })
      u.leafCount = Math.max(1, u.leafCount)
    }
    roots.forEach(computeLeafCount)
    function setPosition(offsetX, offsetY, u) {
      u.old_position = u.position
      u.position =
        { x : offsetX
        , y : offsetY + u.leafCount * NODE_DY / 2 }
      u.children.forEach(function(v) {
        setPosition(offsetX + NODE_DX, offsetY, v)
        offsetY += v.leafCount * NODE_DY
      })
    }
    totalLeafCount = 0
    roots.forEach(function(u) {totalLeafCount += u.leafCount})
    offsetY = HEIGHT / 2 - totalLeafCount * NODE_DY / 2
    roots.forEach(function(u) {
      setPosition(ROOT_X, offsetY, u)
      offsetY += u.leafCount * NODE_DY
    })

    // Compute heights.
    function computeHeight(u) {
      u.children.forEach(computeHeight)
      if (u.active) {
        u.height = 0
      } else {
        u.height = 1000
        u.children.forEach(function(v) { u.height = Math.min(u.height, v.height) })
        u.height += 1
      }
    }
    roots.forEach(computeHeight)

    // Now redraw.
    function get_nx(p) { return p.x - NODE_SIZE / 2 }
    function get_ny(p) { return p.y - NODE_SIZE / 2 }
    function get_lx(p) { return p.x }
    function get_ly(p) { return p.y + NODE_SIZE / 2 - 6 }
    function get_pos(f) { return function(n) { return f(n.position) } }
    function get_old(f) { return function(n) { return f(n.old_position? n.old_position : n.position) } }
    function get_p(f) { return function(n) { return f(n.parent? n.parent : n) } }

    function get_nfill(n) {
      if (n.active) {
        return GREEN
      } else if (n.to_delete) {
        return PURPLE
      } else if (n.height < tree.HISTORY) {
        return YELLOW
      } else {
        return RED
      }
    }
    function label(n) { return n.depth }
    function get(f) { return function(x) { return x[f] } }
    function rep_path(arc) {
      var ay1 = arc.y1 - NODE_SIZE / 2
      var ay2 = arc.y2 - NODE_SIZE / 2
      var c1x = arc.x1 + NODE_SIZE / 4
      var c1y = ay1 - NODE_DY * 0.25
      var c2x = arc.x2 - NODE_SIZE / 4
      var c2y = ay2 - NODE_DY * 0.25
      result =
        'M ' + arc.x1+','+ay1
        + ' C ' + c1x+','+c1y + ' ' + c2x+','+c2y
        + ' ' + arc.x2+','+ay2
      return result
    }
    function rep_loop(arc) {
      return rep_path({x1:arc.x2, y1:arc.y2, x2:arc.x2, y2:arc.y2, id:arc.id})
    }

    var reps = d3.select('#rttb-reps')
      .selectAll('path').data(getRepArcs, get('id'))
    reps.transition().duration(TRANSITION)
      .attr('d', rep_path)
    reps.enter().append('path')
      .attr('d', rep_loop)
      .transition().duration(TRANSITION)
      .attr('d', rep_path)
    reps.exit().remove()

    var parents = d3.select('#rttb-parents')
      .selectAll('line').data(getParentLinks, get('id'))
    parents.transition().duration(TRANSITION)
      .attr('x1',get('x1')).attr('y1',get('y1'))
      .attr('x2',get('x2')).attr('y2',get('y2'))
    parents.enter().append('line')
      .attr('x1',get('x1')).attr('y1',get('y1'))
      .attr('x2',get('x1')).attr('y2',get('y1'))
      .transition().duration(TRANSITION)
      .attr('x2',get('x2')).attr('y2',get('y2'))
    parents.exit().remove()

    var nx = get_pos(get_nx)
    var ny = get_pos(get_ny)
    var onx = get_p(get_old(get_nx))
    var ony = get_p(get_old(get_ny))
    var nodes = d3.select("#rttb-nodes")
      .selectAll('rect').data(tree.nodes, get('id'))
    nodes.transition().duration(TRANSITION)
      .attr('x',nx).attr('y',ny)
      .style('fill', get_nfill)
    nodes.enter().append('rect')
      .attr('x',onx).attr('y',ony)
      .style('fill',get_nfill).attr('height',NODE_SIZE).attr('width',NODE_SIZE)
      .on('click',clickHandler)
      .transition().duration(TRANSITION)
      .attr('x',nx).attr('y',ny)
    nodes.exit().remove()

    var lx = get_pos(get_lx)
    var ly = get_pos(get_ly)
    var olx = get_p(get_old(get_lx))
    var oly = get_p(get_old(get_ly))
    var labels = d3.select("#rttb-labels")
      .selectAll('text').data(tree.nodes, get('id'))
    labels.transition().duration(TRANSITION)
      .attr('x',lx).attr('y',ly)
    labels.enter().append('text')
      .html(label)
      .attr('x',olx).attr('y',oly)
      .on('click',clickHandler)
      .transition().duration(TRANSITION)
      .attr('x',lx).attr('y',ly)
    labels.exit().remove()
  }

  var initialize = function() {
    reset()
    d3.select("#treebuffer-realtime")
      .append("svg").attr("width", WIDTH).attr("height", HEIGHT)
        .attr('id', 'rttb-svg')
    d3.select("#treebuffer-realtime")
      .append('div').attr('id', 'rttb-buttons')
    d3.select("#rttb-buttons")
      .append("button").attr('type', 'button').text('Reset')
      .on('click', function(_) { reset(); redraw() })
    d3.select("#rttb-svg").append('g').attr('id', 'rttb-parents')
    d3.select("#rttb-svg").append('g').attr('id', 'rttb-reps')
    d3.select("#rttb-svg").append('g').attr('id', 'rttb-nodes')
    d3.select("#rttb-svg").append('g').attr('id', 'rttb-labels')
    redraw()
  }

  initialize()
  return tree
}
var realtime_tree = make_realtime_tree()
