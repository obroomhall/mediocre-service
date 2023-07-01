# Pulls an image from docker hub and re-tags it for local development

$repo = "obroomhall/mediocre"
$tagToPull = Read-Host -Prompt "Which tag would you like to pull? [master]"

if (!$tagToPull) {
    $tagToPull = "master"
}

docker pull ${repo}:${tagToPull}

if ($?) {
    Write-Host "Retagging ${repo}:${tagToPull} as mediocre:local"
    docker tag ${repo}:${tagToPull} mediocre:local
}
