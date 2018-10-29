require 'graphviz'

begin
    GraphViz.parse( "tree.dot", :path => "/" ) { |g|
    }.output(:png => "tree.png")
    print("Generated tree.png!\n")
rescue RuntimeError
    print("Unable to generate parsetree.png, file not found.\n")
end
