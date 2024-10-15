#!/bin/bash
# Builds the library using registry cache

dockerhub_repo='obroomhall/mediocre'

tags=$(curl -s https://registry.hub.docker.com/v2/repositories/$dockerhub_repo/tags?page_size=100 \
      | jq -r '.results[].name' \
      | grep -E "^(master|pr-[0-9]+)-develop-cache$")

IFS=$'\n' read -r -d '' -a options_array <<< "$tags"

options_array+=("other")

echo -n "Which recent cache tag would you like to build from?"
echo -e "\n"

COLUMNS=12 # render one choice per line

select choice in "${options_array[@]}"; do
    if [[ $REPLY -gt 0 && $REPLY -le ${#options_array[@]} ]]; then
        if [[ "${options_array[$REPLY-1]}" == "other" ]]; then
            echo -n "Enter your custom choice: "
            read custom_choice
            selected_choice="$custom_choice"
        else
            selected_choice="${options_array[$REPLY-1]}"
        fi

        command=(
          "docker" "buildx" "build"
          "--force-rm"
          "--progress" "plain"
          "--cache-from" "type=registry,ref=$selected_choice"
          "--target" "develop"
          "-t" "mediocre:local-develop"
          "."
        )
        break
    else
        echo "Invalid option. Please try again."
    fi
done

echo -e "\n"
echo "> ${command[@]}"
"${command[@]}"
