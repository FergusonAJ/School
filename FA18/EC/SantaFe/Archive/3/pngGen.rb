require 'graphviz'

begin
    GraphViz.parse( "tree.dot", :path => "/" ) { |g|
    }.output(:png => "tree.png")
    print("Generated tree.png!\n")
rescue RuntimeError
    print("Unable to generate tree.png, file not found.\n")
end

begin
    GraphViz.parse( "modTree.dot", :path => "/" ) { |g|
    }.output(:png => "modTree.png")
    print("Generated modTree.png!\n")
rescue RuntimeError
    print("Unable to generate modTree.png, file not found.\n")
end
