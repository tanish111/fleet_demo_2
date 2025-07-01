GEMINI Agent Workflow
To start, each GEMINI agent identifies itself by the system hostname. Running the hostname command returns the machine’s unique network name
. Use this name to select the tasks intended for you. The instructions.txt file contains build tasks organized in sections bracketed by @. Each section begins with @<hostname>:<commit_code>:{deps} (where <deps> is a list of dependent commit codes) followed by a $ line describing the work, and ends with @. 
You can use this script to extract each item from instruction(any thing between @..@ is a new instruction and can use script to parse)
parse_instruction() {
    local input="$1"
    # Check if input starts with @ and ends with @
    if [[ ! "$input" =~ ^@.*@$ ]]; then
        echo "Error: Invalid format - instruction must be encapsulated in @...@"
        return 1
    fi
    
    # Remove the outer @ symbols
    local content="${input:1:${#input}-2}"
    
    # Check for $ delimiters for text instruction
    if [[ ! "$content" =~ \$.*\$ ]]; then
        echo "Error: Invalid format - text instruction must be encapsulated in \$...\$"
        return 1
    fi
    
    # Extract the text instruction (everything between $ delimiters)
    local text_instruction="${content#*\$}"
    text_instruction="${text_instruction%\$*}"
    
    # Extract parts before the first $
    local before_text="${content%%\$*}"
    
    # Parse the before_text part: <hostname>:<commit-code>:{<dependency_instruction>}
    
    # Split by first colon to get hostname
    if [[ ! "$before_text" =~ : ]]; then
        echo "Error: Missing colon separator for hostname"
        return 1
    fi
    local hostname="${before_text%%:*}"
    local rest="${before_text#*:}"
    # Split rest by colon and curly braces to get commit-code and dependencies
    if [[ ! "$rest" =~ :\{ ]]; then
        echo "Error: Invalid format - expected :<commit-code>:{<dependencies>}"
        return 1
    fi

    local commit_code="${rest%%:*}"
    local deps_part="${rest#*:\{}"
    local dependencies="${deps_part%\}*}"
    
    # Output parsed components
    echo "=== PARSED INSTRUCTION COMPONENTS ==="
    echo "Hostname: $hostname"
    echo "Commit Code: $commit_code"
    echo "Dependencies: $dependencies"
    echo "Text Instruction: $text_instruction"
    echo "===================================="
    
    return 0
}
Strictly filter tasks by matching the <hostname> to your hostname; ignore any instruction blocks not tagged for you. For example, if hostname returns agent1, only process sections that start with @agent1:....
Run hostname: Obtain your agent’s identity (e.g. agent1)
.
Open instructions.txt: Read through the file’s sections demarcated by @.
Select matching tasks: Keep only the blocks whose <hostname> matches your agent’s name. Discard others entirely.
Dependency Verification
Before executing a task, ensure all its dependent commit codes are already in the repository history. Check the list of dependencies (in the {...} field) against your Git log (e.g. using git log --grep=<commit_code>). If any required commit is missing, wait for it by syncing with the remote repository. In practice, after pushing changes you can loop on git pull until new commits appear. For example:
git push
while true; do
    if ! git pull | grep -q "Already up to date."; then
        echo "New changes pulled from the remote branch."
        break
    fi
    sleep 5
done
This loop repeatedly pulls from origin/master until it fetches updates (i.e. until git pull outputs something other than “Already up to date.”). Only proceed with the task once all dependent commit codes are present in the history.
Building and Committing Tasks
Now implement the task as described (the exact build steps are assumed generic and not language-specific). When the work is complete, stage and commit your changes. The commit message must begin with the given commit code, followed by a hyphen, your hostname, another hyphen, and then a brief title. For example:
c501889e5764dc579f6afcf1507f07cbaa9db45f3aac245e15aca60cda7f3313-agent1-ImplementLoginFeature
This format ties the commit to the instruction’s code and the agent. After preparing the commit message, execute the commit and then push it to the shared repository. In Git terms, you record the snapshot locally and then push it so others can see it
.
Use git add <files> to stage your changes.
Commit with: git commit -m "<commit_code>-<hostname>-Title". Include a detailed body/description as needed after the title.
Immediately run git push origin master to upload your commit to the remote repository
Synchronizing Changes
After each push, synchronize your local branch with origin/master. Run git pull origin master, which fetches and merges any new commits from others
. Since git pull combines a fetch and merge, it applies remote changes on top of your work. If a merge conflict occurs, accept the incoming (remote) changes as needed and finalize the merge. Edit any conflicted files to the desired state (in this protocol, that means keeping the incoming changes), then create a new commit of the resolved state. Handling conflicts promptly ensures all agents stay in sync
.
Immediately after pushing, run git pull origin master. This fetches and merges new updates
.
If Git reports merge conflicts, resolve them by using the incoming (remote) changes and then commit the resolution
.
Once merged successfully, continue to the next instruction.
Generating New Instructions
If you complete all tasks for your hostname and find no further instructions, the agent should generate more. Use an online search (e.g. Google Search) or other knowledge tools to identify additional relevant tasks for the project. For each new task you create:
Write the instruction in the same @<hostname>:<commit_code>:{deps} ... @$ format. Make sure it is not a duplicate of any existing instruction.
Validate the format by running ./format_checker.sh <instruction>. Only add it to instructions.txt if the script outputs PASSED; otherwise, revise the format and try again.
Generate commit codes using ./random_generator.sh -n N to produce the needed number of new codes (each is a unique hex string). Each instruction line must use one of these codes.
After generating instructions, distribute them evenly among all agent hostnames to balance work. For example, if you create 4 new tasks and have 2 agents (agent1 and agent2), you might assign 2 tasks to each. Assign by editing the <hostname> field on each new instruction. This ensures tasks are shared and non-overlapping.
Append to instructions.txt don't remove any old instruction from it.
Use search tools (e.g. Google) to find fresh tasks related to the project scope.
Ensure new tasks don’t repeat content already in instructions.txt.
For each valid instruction, append it under instructions.txt once it passes the format check (PASSED).
Assign each new instruction to a hostname so that work is evenly split across agents.
Additional Guidelines
Remote and Branch: Always assume the Git remote is named origin and the branch is master. Push and pull using origin master.
Hostname Filtering: Strictly ignore any instructions not matching your hostname. Don’t attempt to do tasks meant for other agents.
Up-to-Date Sync: Keep your local repo synced with origin/master at all times. For example, pull the latest changes before creating a commit. This “always synchronize” practice helps avoid conflicts
.
Non-Overlapping Work: Coordinate (via the instructions.txt system) so agents work on separate tasks. Avoid overlapping code changes to minimize merge conflicts.
By following these protocols, each GEMINI agent will reliably pick up its own tasks, commit and share work properly
, and keep all code in sync
.