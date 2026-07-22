#! /bin/bash
echo "## Project layout:" > architecture.md
# get structure of project with tree and use sed to get valid newlines for Markdown
tree ../. -a -I '.git|build/*|bin/*|.gitignore|*.o|*.d|*.a|world_data/*' | sed 's/$/  /' >> architecture.md

echo "  " >> architecture.md
echo "## Lines of code by file:  " >> architecture.md

find ../ \( -name "*.c" -o -name "*.h" \) -exec wc -l {} + | sed 's/$/  /'  >> architecture.md
